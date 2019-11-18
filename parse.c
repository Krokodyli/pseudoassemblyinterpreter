#include "basicFunctions.h"
#include "lists.h"
#include "parse.h"

int parse_word(char** out, mString* mstr, int* mstr_size){
    mString_to_char_array(mstr, *mstr_size, out);
    *mstr_size = 0;
    return check_if_char_array_is_connected(*out);
}

int parse_integer(int* out, mString* mstr, int* mstr_size){ 
   int failed = mString_parse_int(out, *mstr);

   mString_clear(mstr);
   *mstr_size = 0;

   return failed;
}

void parse_label_and_operation_type(char cc, mString* mstr, int* mstr_size, char** label, int* op_inf, int* parsing_flag, int line){
    int i;
    if(cc != ' '){
        mString_add(cc, mstr);
        (*mstr_size)++;
    }
    else{
        char* firstWord;
        if(parse_word(&firstWord, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: unexpected space character", line);
        if(firstWord[0] != 0){
            for(i = 0; i < MEMORY_INSTRUCTIONS_COUNT; i++){
                if(str_cmp(firstWord, MEMORY_INSTRUCTIONS[i]) == 0){
                    *parsing_flag = 10; 
                    *op_inf = hash_instruction(firstWord);
                    free(firstWord);
                    break;
                }
            }
            if(*op_inf == 0){
                for(i = 0; i < OPERATIONS_INSTRUCTIONS_COUNT; i++){
                    if(str_cmp(firstWord, OPERATIONS_INSTRUCTIONS[i]) == 0){
                        *parsing_flag = 20; 
                        *op_inf = hash_instruction(firstWord);
                        if((*op_inf)%100 == 18) {
                            *parsing_flag = 20;
                        }
                        else if((*op_inf)/100 == 10) {
                            *parsing_flag = 40;
                        }
                        else {
                            *parsing_flag = 30;
                        }
                        free(firstWord);
                        break;
                    }
                }
            }
            assert_syntax_error(*parsing_flag != 1, "Syntax error: unexpected space character", line);
            if(*parsing_flag == 0){
                *parsing_flag = 1;
                *label = firstWord;
            }
        }
    }
}

void parse_memory_instruction(char cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, int* parsing_flag, int line){
    char* word;
    if(cc == '*'){
        assert_syntax_error(*parsing_flag == 10, "Syntax error: unexpected '*' character", line);
        
        if(parse_integer(arg1_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line);
        assert_syntax_error(*arg1_inf > 0, "Syntax error: incorrect array size", line);
            
        *parsing_flag = 11;
    }
    else if(cc == '('){
        assert_syntax_error(*parsing_flag < 12, "Syntax error: unexpected '(' character", line);

        if(parse_word(&word, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: unexpected space character", line);
        assert_syntax_error(str_cmp(word, "INTEGER") == 0, "Syntax error: expected keyword 'INTEGER'", line);

        free(word);

        *parsing_flag = 12;
    }
    else if(cc == ')'){
        assert_syntax_error(*parsing_flag == 12, "Syntax error: unexpected ')' character", line);

        if(parse_integer(arg2_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line);

        *parsing_flag = 13;
    }
    else {
        mString_add(cc, mstr);
        (*mstr_size)++;
    }
}

void parse_register_register_operation(char cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, int* parsing_flag, int line){
    if(cc == ','){
        assert_syntax_error(*parsing_flag == 20, "Syntax error: expected integer", line);

        if(parse_integer(arg1_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line);

        *parsing_flag = 21;
    }
    else{
        mString_add(cc, mstr);
        (*mstr_size)++;
    }

}

void parse_register_memory_operation(int cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, char** arg_label, int* parsing_flag, int line){
    if(cc == ','){
        assert_syntax_error(*parsing_flag == 30 || *parsing_flag == 33, "Syntax error: unexpected ',' character", line);

        if(parse_integer(arg1_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line); 
        *parsing_flag = 31;
    }
    else if(cc == '('){
        assert_syntax_error(*parsing_flag == 31, "Syntax error: unexpected '(' character", line);

        if(parse_word(arg_label, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: unexpected space character", line);

        *parsing_flag = 32;
    }
    else if(cc == ')'){
        assert_syntax_error(*parsing_flag == 32, "Syntax error: unexpected ')' character", line);

        if(parse_integer(arg2_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line);

        assert_syntax_error(*arg2_inf >= 0 && *arg2_inf < REGISTER_SIZE, "Syntax error: expected register number", line);

        *parsing_flag = 33;
    }
    else{
        mString_add(cc, mstr);
        (*mstr_size)++;
    }
}

void parse_jump_instruction(char cc, mString* mstr, int* mstr_size, int* arg1_inf, char** arg_label, int* parsing_flag, int line){
    if(cc == '('){
        assert_syntax_error(*parsing_flag == 40, "Syntax error: unexpected '(' character", line);
        
        if(parse_word(arg_label, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: unexpected space character", line);

        *parsing_flag = 41;
    }
    else if(cc == ')'){
        assert_syntax_error(*parsing_flag == 41, "Syntax error: unexpected ')' character", line);
        
        if(parse_integer(arg1_inf, mstr, mstr_size) == 0)
            print_syntax_error("Syntax error: expected integer", line);
        
        *parsing_flag = 42;
    }
    else {
        mString_add(cc, mstr);
        (*mstr_size)++;
    }
}

void parse_line(FILE* file, char** line_str, char** label, int* op_inf, int* arg1_inf, int* arg2_inf, char** arg_label, int line){
    int parsing_flag = 0;
    char* word;
    char cc = 0;
    mString mstr = NULL;
    int mstr_size = 0;
    mString mstr_line = NULL;
    int mstr_line_size = 0;

    *arg_label = NULL;
    *label = NULL;
    *line_str = NULL;
    *op_inf = 0;
    *arg1_inf = 0;
    *arg2_inf = 0;

    cc = fgetc(file);
    while(cc != EOF && cc != '\n'){
        mString_add(cc, &mstr_line);
        mstr_line_size++;

        if(parsing_flag < 2){
            parse_label_and_operation_type(cc, &mstr, &mstr_size, label, op_inf, &parsing_flag, line);
        }
        else if(parsing_flag < 15){
            parse_memory_instruction(cc, &mstr, &mstr_size, arg1_inf, arg2_inf, &parsing_flag, line);
        }
        else if(parsing_flag < 30){
            parse_register_register_operation(cc, &mstr, &mstr_size, arg1_inf, arg2_inf, &parsing_flag, line);
        }
        else if(parsing_flag < 40){
            parse_register_memory_operation(cc, &mstr, &mstr_size, arg1_inf, arg2_inf, arg_label, &parsing_flag, line);
        }
        else if(parsing_flag < 50){
            parse_jump_instruction(cc, &mstr, &mstr_size, arg1_inf, arg_label, &parsing_flag, line);
        }
        cc = fgetc(file);
    }
    if(mstr != NULL){
       if(parsing_flag == 10 || parsing_flag == 11){ 
            if(parse_word(&word, &mstr, &mstr_size) == 0)
                print_syntax_error("Syntax error: unexpected space character", line);

            assert_syntax_error(str_cmp(word, "INTEGER") == 0, "Syntax error: expected keyword 'INTEGER'", line);

            free(word);
            parsing_flag = 13;
       }
       if(parsing_flag == 21){
            if(parse_integer(arg2_inf, &mstr, &mstr_size) == 0)
                print_syntax_error("Syntax error: expected integer", line);
            parsing_flag = 22;
       }
       if(parsing_flag == 31){
            if(parse_word(arg_label, &mstr, &mstr_size) == 0)
                print_syntax_error("Syntax error: unexpected space character", line);
            parsing_flag = 33;
            *arg2_inf = -1;
       }
       else if(parsing_flag == 40){
            if(parse_word(arg_label, &mstr, &mstr_size) == 0)
                print_syntax_error("Syntax error: unexpected space character", line);
            parsing_flag = 42;
       }
    }
    assert_syntax_error(parsing_flag == 0 || parsing_flag == 13 || parsing_flag == 22 || parsing_flag == 33 || parsing_flag == 42, "Syntax error: unexpected end of the line", line);
    parse_word(line_str, &mstr_line, &mstr_line_size);
}

pseudoassembly_program* parse_file(char* path){
    int i;
    int operations_addresses_it;
    int memory_addresses_it;
    int memory_alloc_it;

    char* line_str;
    char* label;
    char* arg_label;
    int op_inf, arg1_inf, arg2_inf;
    instruction_list ins_list = NULL;
    int memory_instructions_count = 0;
    int operations_instructions_count = 0;
    int memory_allocated = 0;
    int line = 1;
    instruction_list ins_list_it;

    pseudoassembly_program* pa_program;

    FILE* file = fopen(path, "r");
    if(file == NULL){
        print_error("No such file");
    }
    while(!feof(file)){
        parse_line(file, &line_str, &label, &op_inf, &arg1_inf, &arg2_inf, &arg_label, line);
        if(op_inf == 0){
            line++;
            continue;
        }
        else if(op_inf == 403 || op_inf == 419){
            memory_instructions_count++;
            if(arg1_inf == 0)
                memory_allocated++;
            else
                memory_allocated += arg1_inf;
        }
        else{
            operations_instructions_count++;
        }
        fix_char_array(&arg_label);
        fix_char_array(&label);
        instruction_list_add(line_str, label, op_inf, arg1_inf, arg2_inf, arg_label, line, &ins_list);
        line++;
    }
    fclose(file);
   
    pa_program = (pseudoassembly_program*)malloc(sizeof(struct pseudoassembly_program));
    pa_program->operations_addresses = (int*)malloc(sizeof(int)*operations_instructions_count);
    pa_program->operations_labels = (char**)malloc(sizeof(char*)*operations_instructions_count);
    pa_program->memory_addresses = (int*)malloc(sizeof(int)*memory_instructions_count);
    pa_program->memory_labels = (char**)malloc(sizeof(char*)*memory_instructions_count);
    pa_program->memory = (int*)malloc(sizeof(int)*(memory_allocated+OP_SIZE*operations_instructions_count));
    pa_program->code = (char**)malloc(sizeof(char*)*operations_instructions_count);
    pa_program->current_operation_address = memory_allocated;
    pa_program->last_operation_address = memory_allocated;
    pa_program->memory_instructions_count = memory_instructions_count;
    pa_program->operations_instructions_count = operations_instructions_count;
    pa_program->memory_allocated = memory_allocated;
    operations_addresses_it = operations_instructions_count - 1;
    memory_addresses_it = memory_instructions_count - 1;
    memory_alloc_it = memory_allocated;
    ins_list_it = ins_list;

    for(i = 0; i < REGISTER_SIZE; i++) pa_program->register_memory[i] = 0;
    pa_program->FLAG_SILENT = 0;
    pa_program->FLAG_INTERVAL = 0;
    pa_program->FLAG_NORMAL = 0;
    pa_program->FLAG_LAST = 0;

    while(ins_list_it != NULL){
        if(ins_list_it->command == 403 || ins_list_it->command == 419){
            if(ins_list_it->arg1 == 0){
                memory_alloc_it--;
            }
            else{
                memory_alloc_it -= ins_list_it->arg1;
            }

            pa_program->memory_addresses[memory_addresses_it] = memory_alloc_it;
            pa_program->memory_labels[memory_addresses_it] = ins_list_it->label;
            memory_addresses_it--;
            fill_variable_memory(ins_list_it->command, ins_list_it->arg1, ins_list_it->arg2, memory_alloc_it, ins_list_it->line, pa_program);
        }
        else{
            pa_program->operations_addresses[operations_addresses_it] = memory_allocated + operations_addresses_it*4;
            pa_program->operations_labels[operations_addresses_it] = ins_list_it->label;
            operations_addresses_it--;
        }
        ins_list_it = ins_list_it -> next_node;
    }

    operations_addresses_it = pa_program->operations_instructions_count - 1;
    while(ins_list != NULL){
        instruction_list oldPointer = ins_list;
        if(ins_list->command != 403 && ins_list->command != 419 && ins_list->command != 0){
            fill_operation_memory(ins_list->command, ins_list->arg1, ins_list->arg2, ins_list->arg_label, memory_allocated + OP_SIZE*operations_addresses_it, ins_list->line, pa_program);
            pa_program->code[operations_addresses_it] = ins_list->line_str;
            operations_addresses_it--;
        }
        ins_list = ins_list->next_node;
        free(oldPointer->arg_label);
        free(oldPointer);
    }
    return pa_program;
}


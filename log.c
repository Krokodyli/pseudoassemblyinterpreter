#include "log.h"

void assert(int assertion, char* functionName){
    if(assertion == 0){
        set_console_color(C_COLOR_RED);
        printf("Assertion failed in function %s.\n", functionName); exit(EXIT_FAILURE);
        set_console_color(C_COLOR_WHITE);
    }
}

void print_syntax_error(char* message, int line){
    set_console_color(C_COLOR_RED);
    printf("%s (line %d)\n", message, line);
    set_console_color(C_COLOR_WHITE);
    exit(EXIT_FAILURE);
}

void assert_syntax_error(int assertion, char* message, int line){
    if(assertion == 0){
        set_console_color(C_COLOR_RED);
        printf("%s (line %d)\n", message, line);
        set_console_color(C_COLOR_WHITE);
        exit(EXIT_FAILURE);
    }
}

void print_error(char* message){
    set_console_color(C_COLOR_RED);
    printf("%s\n", message);
    set_console_color(C_COLOR_WHITE);
    exit(EXIT_FAILURE);
}

void set_gui_size(int height, int variables_count, int* code_window, int* register_window, int* variables_window){
    int rem_height = height - LOG_ALWAYS_NEEDED_SIZE - variables_count;
    if(rem_height < LOG_SIZE_TIER2){
        *code_window = LOG_SIZE_TIER1_CODE;
        *register_window = LOG_SIZE_TIER1_REGISTER;
        *variables_window = height - LOG_ALWAYS_NEEDED_SIZE - (*code_window)*2 - LOG_REGISTER_SIZE/(*register_window);
        if(LOG_REGISTER_SIZE%(*register_window) != 0)
            (*variables_window)--;
        if(*variables_window < 0) *variables_window = 0;
    }
    else if(rem_height < LOG_SIZE_TIER3){
        *code_window = LOG_SIZE_TIER2_CODE;
        *register_window = LOG_SIZE_TIER2_REGISTER;
        *variables_window = variables_count;
    }
    else if(rem_height < LOG_SIZE_TIER4){
        *code_window = LOG_SIZE_TIER3_CODE;
        *register_window = LOG_SIZE_TIER3_REGISTER;
        *variables_window = variables_count;
    }
    else if(rem_height < LOG_SIZE_TIER5){
        *code_window = LOG_SIZE_TIER4_CODE;
        *register_window = LOG_SIZE_TIER4_REGISTER;
        *variables_window = variables_count;
    }
    else if(rem_height < LOG_SIZE_TIER6){
        *code_window = LOG_SIZE_TIER5_CODE;
        *register_window = LOG_SIZE_TIER5_REGISTER;
        *variables_window = variables_count;
    }
    else{
        *code_window = LOG_SIZE_TIER6_CODE;
        *register_window = LOG_SIZE_TIER6_REGISTER;
        *variables_window = variables_count;
    }
}

void log_operation(pseudoassembly_program* pa_program){
    int i,j;
    int height = get_terminal_height();
    int width = get_terminal_width();
    int code_window_size;
    int register_window_size;
    int variables_window_size;
    int rest;
    int operation_it = binsearch(pa_program->operations_addresses, pa_program->operations_instructions_count, pa_program->last_operation_address);

    set_gui_size(height, pa_program->memory_instructions_count, &code_window_size, &register_window_size, &variables_window_size);

    set_console_color(C_COLOR_LIGHT_BLUE);
    rest = (width-4)%2;
    for(i = 0; i < (width-4)/2; i++) printf("=");
    set_console_color(C_COLOR_LIGHT_RED);
    printf("CODE");
    set_console_color(C_COLOR_LIGHT_BLUE);
    for(i = 0; i < (width-4)/2+rest; i++) printf("=");

    for(i = operation_it - code_window_size; i <= operation_it + code_window_size; i++){
        if(i == operation_it){
            set_console_color(C_COLOR_LIGHT_GREEN);
            printf("%*.*s ", -width/2+2, width/2-2, pa_program->code[i]);
            printf("[%d]\n", pa_program->program_state);
            set_console_color(C_COLOR_LIGHT_BLUE);
        }
        else if(i >= 0 && i <= pa_program->operations_instructions_count-1){
            printf("%*.*s\n", -width/2+2, width/2-2, pa_program->code[i]);
        }
        else{
            printf(" \n");
        }
    }
    for(i = 0; i < width; i++) printf("=");
    printf("\n");

    rest = (width-8)%2;
    for(i = 0; i < (width-8)/2; i++) printf("=");
    set_console_color(C_COLOR_LIGHT_RED);
    printf("REGISTER");
    set_console_color(C_COLOR_LIGHT_BLUE);
    for(i = 0; i < (width-8)/2+rest; i++) printf("=");

    for(i = 0; i < LOG_REGISTER_SIZE; i += register_window_size){
        for(j = 0; j < register_window_size && i+j < LOG_REGISTER_SIZE; j++){
            printf("==");
            set_console_color(C_COLOR_LIGHT_GREEN);
            printf("%02d", i+j);
            set_console_color(C_COLOR_LIGHT_BLUE);
            printf("==");
            set_console_color(C_COLOR_LIGHT_GREEN);
            printf("%*d", INTEGER_WIDTH, pa_program->register_memory[i+j]);
            set_console_color(C_COLOR_LIGHT_BLUE);
            printf("==");
            if(j != register_window_size - 1)
                printf("     |     ");
        }
        printf("\n");
    }
    for(i = 0; i < width; i++) printf("=");
    rest = (width-9)%2;
    for(i = 0; i < (width-9)/2; i++) printf("=");
    set_console_color(C_COLOR_LIGHT_RED);
    printf("VARIABLES");
    set_console_color(C_COLOR_LIGHT_BLUE);
    for(i = 0; i < (width-9)/2+rest; i++) printf("=");
    register_window_size = LOG_REGISTER_SIZE/register_window_size;
    if(LOG_REGISTER_SIZE%register_window_size != 0) register_window_size++;
    for(i = 0; i < variables_window_size; i++){
        printf("==");
        set_console_color(C_COLOR_LIGHT_GREEN);
        printf("%*.*s", -width/3, width/3, pa_program->memory_labels[i]);
        set_console_color(C_COLOR_LIGHT_BLUE);
        printf("==");
        set_console_color(C_COLOR_LIGHT_GREEN);
        printf("%*d", INTEGER_WIDTH, pa_program->memory[pa_program->memory_addresses[i]]);
        set_console_color(C_COLOR_LIGHT_BLUE);
        printf("==\n");
    }

    for(i = 0; i < width; i++) printf("=");
    printf("\n");

    clear_rest_screen(LOG_ALWAYS_NEEDED_SIZE + code_window_size*2 + register_window_size + variables_window_size - 2);
}

void log_program(pseudoassembly_program* pa_program, int* ins_to_skip, int* operation_address_log, int last){
    if(!pa_program->FLAG_SILENT && !last){
        if((*ins_to_skip == 0 && *operation_address_log == -1) || pa_program->last_operation_address == *operation_address_log){
           log_operation(pa_program);
           if(pa_program->FLAG_NORMAL)
               execute_command(pa_program, ins_to_skip, operation_address_log);
           if(pa_program->FLAG_INTERVAL)
               system_sleep(pa_program->FLAG_INTERVAL);

        }
        else if(*ins_to_skip > 0){
            (*ins_to_skip)--;
        }
    }
    if(last && pa_program->FLAG_LAST){
        log_operation(pa_program);
    }
}

int parse_command_argument(char buff[COMMAND_BUFF_SIZE], char label_buff[COMMAND_BUFF_SIZE], char arg_buff[COMMAND_BUFF_SIZE], int* offset){
    int it = 0;
    int end_label_flag = -1;
    while(buff[it] != '\0'){
        if(end_label_flag == -1){
            if(buff[it] != '('){
                label_buff[it] = buff[it];
            }
            else{
                end_label_flag = it + 1;
            }
        }
        else{
            if(buff[it] != ')'){
                arg_buff[it-end_label_flag] = buff[it];
            }
            else{
                if(buff[it+1] == '\0'){
                    arg_buff[it-end_label_flag] = '\0';
                    break;
                }
                else
                    return 0;
            }
        }
        it++;
    }
    *offset = 0;
    if(arg_buff[0] != '\0'){
        if(!char_array_to_int(offset, arg_buff)){
            return 0;
        }
    }
    return 1;
}

void execute_command(pseudoassembly_program* pa_program, int* ins_to_skip, int* operation_address_to_log){
    int i, val, offset, flag_is_label;
    char buff[COMMAND_BUFF_SIZE];
    char label_buff[COMMAND_BUFF_SIZE] = { 0 };
    char arg_buff[COMMAND_BUFF_SIZE] = { 0 };
    *ins_to_skip = 0;
    *operation_address_to_log = -1;
    while(1){
        set_console_color(C_COLOR_LIGHT_BLUE);
        printf("COMMAND: ");
        set_console_color(C_COLOR_LIGHT_GREEN);
        scanf("%s", buff);
        if(strcmp(buff, "n") == 0 || strcmp(buff, "next") == 0){
            ins_to_skip = 0;
            break;
        }
        else if(strcmp(buff, "s") == 0 || strcmp(buff, "skip") == 0){
            val = 0;
            scanf("%s", buff);
            if(char_array_to_int(&val, buff) && val > 0){
                *ins_to_skip = val;
                break;
            }
            else{
                set_console_color(C_COLOR_LIGHT_BLUE);
                printf("Wrong argument\n");
                continue;
            }
        }
        else if(strcmp(buff, "p") == 0 || strcmp(buff, "print") == 0){
            scanf("%s", buff);
            for(i = 0; i < COMMAND_BUFF_SIZE; i++){
                label_buff[i] = 0;
                arg_buff[i] = 0;
            }
            offset = 0;
            if(parse_command_argument(buff, label_buff, arg_buff, &offset) == 0){
                set_console_color(C_COLOR_LIGHT_BLUE);
                printf("Wrong argument\n");
                continue;
            }
            flag_is_label = 0;
            for(i = 0; i < pa_program->memory_instructions_count; i++){
                if(pa_program->memory_labels[i] != NULL && strcmp(pa_program->memory_labels[i], label_buff) == 0){
                    show_integer(label_buff, offset, pa_program->memory[pa_program->memory_addresses[i]+offset/4]);
                    flag_is_label = 1;
                    break;
                }
            }
            if(flag_is_label == 1) continue;
            if(!char_array_to_int(&val, label_buff)){
                set_console_color(C_COLOR_LIGHT_BLUE);
                printf("Wrong argument\n");
                continue;
            }
            else{
                show_integer(label_buff, offset, pa_program->memory[val/4+offset/4]);
            }
        }
        else if(strcmp(buff, "g") == 0 || strcmp(buff, "goto") == 0){
            scanf("%s", buff);
            for(i = 0; i < COMMAND_BUFF_SIZE; i++){
                label_buff[i] = 0;
                arg_buff[i] = 0;
            }
            offset = 0;
            if(parse_command_argument(buff, label_buff, arg_buff, &offset) == 0){
                set_console_color(C_COLOR_LIGHT_BLUE);
                printf("Wrong argument\n");
                continue;
            }
            flag_is_label = 0;
            for(i = 0; i < pa_program->operations_instructions_count; i++){
                if(pa_program->operations_labels[i] != NULL && strcmp(pa_program->operations_labels[i], label_buff) == 0){
                    *operation_address_to_log = pa_program->operations_addresses[i] + offset*4;
                    flag_is_label = 1;
                    break;
                }
            }
            if(!flag_is_label){
                set_console_color(C_COLOR_LIGHT_BLUE);
                printf("Wrong argument\n");
            }
            else{
                break;
            }
        }
        else if(strcmp(buff, "q") == 0 || strcmp(buff, "quit") == 0){
            exit(EXIT_SUCCESS);
        }
        else if(strcmp(buff, "h") == 0 || strcmp(buff, "help") == 0){
            set_console_color(C_COLOR_LIGHT_BLUE);
            printf("COMMANDS: \n");
            printf("n/next - shows next operation\n");
            printf("s/skip <N> - skips <N> operations and shows next operation\n");
            printf("p/print <LABEL>(<OFFSET IN BYTES>)/<LABEL> - prints the value of variable with label <LABEL>\n");
            printf("g/goto <LABEL> - skips until operations with label <LABEL> is being executed\n");
            printf("q/quit - quits the program");
            printf("h/help - shows list of command \n");

        }
        else{
            scanf("%s", buff);
            set_console_color(C_COLOR_LIGHT_BLUE);
            printf("No such command\n");
            printf("Type h or help to show available commands\n");
        }
    }
}

void parse_flags(pseudoassembly_program* pa_program, int argc, char* argv[]){
    if(argc == 2){
        pa_program->FLAG_SILENT = 0;
        pa_program->FLAG_INTERVAL = 0;
        pa_program->FLAG_LAST = 0;
        pa_program->FLAG_NORMAL = 1;
    }
    else if(argc == 3){
        if(strcmp(argv[1], "--last-state") == 0 || strcmp(argv[1], "-l") == 0){
            pa_program->FLAG_SILENT = 1;
            pa_program->FLAG_INTERVAL = 0;
            pa_program->FLAG_LAST = 1;
            pa_program->FLAG_NORMAL = 0;
        }
        else if(strcmp(argv[1], "--silent") == 0 || strcmp(argv[1], "-s") == 0){
            pa_program->FLAG_SILENT = 1;
            pa_program->FLAG_INTERVAL = 0;
            pa_program->FLAG_LAST = 0;
            pa_program->FLAG_NORMAL = 0;
        }
        else{
            print_error("Invalid arguments");
        }
    }
    else if(argc == 4){
        if((strcmp(argv[1], "--interval") == 0 || strcmp(argv[1], "-i") == 0)){
            pa_program->FLAG_SILENT = 0;
            if(!char_array_to_int(&pa_program->FLAG_INTERVAL, argv[2])){
                print_error("Invalid arguments");
            }
            pa_program->FLAG_LAST = 0;
            pa_program->FLAG_NORMAL = 0;
        }
        else{
            print_error("Invalid arguments");
        }
    }
    else{
        print_error("Invalid arguments");
    }
}

int get_integer(char* label, int offset){
    char buff[COMMAND_BUFF_SIZE] = "NO INTEGER";
    int value = 0;
    while(!char_array_to_int(&value, buff)){
        set_console_color(C_COLOR_LIGHT_BLUE);
        if(offset == 0)
            printf("VALUE OF %s: ", label);
        else
            printf("VALUE OF %s(%d): ", label, offset);
        set_console_color(C_COLOR_LIGHT_GREEN);
        scanf("%s", buff);
    }
    return value;
}

void show_integer(char* label, int offset, int integer){
    set_console_color(C_COLOR_LIGHT_BLUE);
    if(offset == 0)
        printf("VALUE OF %s: ", label);
    else
        printf("VALUE OF %s(%d): ", label, offset);
    set_console_color(C_COLOR_LIGHT_GREEN);
    printf("%d\n", integer);
    set_console_color(C_COLOR_LIGHT_BLUE);
}

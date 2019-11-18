#include "pai.h"

char MEMORY_INSTRUCTIONS[MEMORY_INSTRUCTIONS_COUNT][MAX_INS_SIZE] = { "DC", "DS" };
char OPERATIONS_INSTRUCTIONS[OPERATIONS_INSTRUCTIONS_COUNT][MAX_INS_SIZE] = { "A", "AR", "M", "MR", "S", "SR", "D", "DR", "L", "LR", "LA", "J", "JN", "JZ", "JP", "ST", "C", "CR"};

void free_program_memory(pseudoassembly_program* pa_program){
    int i;
    free(pa_program->memory);
    for(i=0; i < pa_program->operations_instructions_count; i++){
        free(pa_program->operations_labels[i]);
        free(pa_program->code[i]);
    }
    free(pa_program->code);
    free(pa_program->operations_addresses);
    free(pa_program->operations_labels);
    for(i = 0; i < pa_program->memory_instructions_count; i++)
        free(pa_program->memory_labels[i]);
    free(pa_program->memory_addresses);
    free(pa_program->memory_labels);
    free(pa_program);
    pa_program = NULL;
}

void fill_variable_memory(int command, int arg1, int arg2, int addr_offset, int line, pseudoassembly_program* pa_program){
    int i;
    if(command == 419){
        assert_syntax_error(arg2 == 0, "Syntax error: unexpected value in instruction 'DS'", line);
        return;
    }
    if(arg1 == 0) arg1 = 1;

    for(i = 0; i < arg1; i++){
        pa_program->memory[addr_offset + i] = arg2;
    }
}

void fill_operation_memory(int command, int arg1, int arg2, char* arg_label, int addr_offset, int line, pseudoassembly_program* pa_program){
    int i, arg1_set = 0, arg2_set = 0, value;
    pa_program->memory[addr_offset] = command;
    if(command % 100 == 18){
       pa_program->memory[addr_offset+1] = arg1;
       pa_program->memory[addr_offset+2] = arg2;
       pa_program->memory[addr_offset+3] = 0;
    }
    else if(command / 100 == 10){
        pa_program->memory[addr_offset+1] = arg1 * 4;
        pa_program->memory[addr_offset+2] = 0;
        pa_program->memory[addr_offset+3] = 0;
        for(i = 0; i < pa_program->operations_instructions_count; i++){
            if(pa_program->operations_labels[i] != NULL && strcmp(arg_label, pa_program->operations_labels[i]) == 0){
                pa_program->memory[addr_offset+1] += pa_program->operations_addresses[i];
                arg1_set = 1;
                break;
            }
        }
        assert_syntax_error(arg1_set == 1, "Syntax error: unknown label", line);
    }
    else{
       pa_program->memory[addr_offset+1] = arg1;
       pa_program->memory[addr_offset+3] = arg2;
       for(i = 0; i < pa_program->memory_instructions_count; i++){
           if(strcmp(arg_label, pa_program->memory_labels[i]) == 0){
                pa_program->memory[addr_offset+2] = pa_program->memory_addresses[i];
                arg2_set = 1;
                break;
           }
       }
       if(arg2_set == 0){
           if(!char_array_to_int(&value, arg_label)){
                print_syntax_error("Syntax error: unknown label", line);
           }
           assert_syntax_error(value != -1, "Syntax error: unknown label", line);
           pa_program->memory[addr_offset+2] = value;
       }
    }
}

void get_input(pseudoassembly_program* pa_program){
    int i, j;
    int lines = 0;
    for(i = 0; i < pa_program->memory_instructions_count; i++){
        if(is_variable_input(pa_program->memory_labels[i])){
            if(i != pa_program->memory_instructions_count - 1){
                for(j = pa_program->memory_addresses[i]; j < pa_program->memory_addresses[i+1]; j++){
                    pa_program->memory[j] = get_integer(pa_program->memory_labels[i], j - pa_program->memory_addresses[i]);
                    lines++;
                }
            }
            else{
                for(j = pa_program->memory_addresses[i]; j < pa_program->memory_allocated; j++){
                    pa_program->memory[j] = get_integer(pa_program->memory_labels[i], j - pa_program->memory_addresses[i]);
                    lines++;
                }
            }
        }
    }
}

void show_output(pseudoassembly_program* pa_program){
    int i, j;
    int lines = 0;
    for(i = 0; i < pa_program->memory_instructions_count; i++){
        if(is_variable_output(pa_program->memory_labels[i])){
            if(i != pa_program->memory_instructions_count - 1){
                for(j = pa_program->memory_addresses[i]; j < pa_program->memory_addresses[i+1]; j++){
                    show_integer(pa_program->memory_labels[i], j - pa_program->memory_addresses[i], pa_program->memory[j]);
                    lines++;
                }
            }
            else{
                for(j = pa_program->memory_addresses[i]; j < pa_program->memory_allocated; j++){
                    show_integer(pa_program->memory_labels[i], j - pa_program->memory_addresses[i], pa_program->memory[j]);
                    lines++;
                }
            }
        }
    }
}

void run_instruction(pseudoassembly_program* pa_program){
    int command = pa_program->memory[pa_program->current_operation_address];
    int arg1 = pa_program->memory[pa_program->current_operation_address+1];
    int arg2 = pa_program->memory[pa_program->current_operation_address+2];
    int arg3 = pa_program->memory[pa_program->current_operation_address+3];
    char* op_name;

    pa_program->last_operation_address = pa_program->current_operation_address;

    op_name = pa_program->operations_labels[binsearch(pa_program->operations_addresses, pa_program->operations_instructions_count, pa_program->current_operation_address)];
    if(op_name == NULL) op_name = "";

    switch(command){
        case A_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] + pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case AR_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] + pa_program->register_memory[arg2];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case C_PCOMMAND:
            pa_program->program_state =  pa_program->register_memory[arg1] - pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            break;
        case CR_PCOMMAND:
            pa_program->program_state = pa_program->register_memory[arg1] - pa_program->register_memory[arg2];
            break;
        case D_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] / pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case DR_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] / pa_program->register_memory[arg2];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case J_PCOMMAND:
            pa_program->current_operation_address = arg1 - OP_SIZE;
            pa_program->program_state = 1;
            break;
        case JN_PCOMMAND:
            if(pa_program->program_state < 0){
                pa_program->current_operation_address = arg1 - OP_SIZE;
                pa_program->program_state = 1;
            }
            else pa_program->program_state = 0;
            break;
        case JP_PCOMMAND:
            if(pa_program->program_state > 0){
                pa_program->current_operation_address = arg1 - OP_SIZE;
                pa_program->program_state = 1;
            }
            else pa_program->program_state = 0;
            break;
        case JZ_PCOMMAND:
            if(pa_program->program_state == 0){
                pa_program->current_operation_address = arg1 - OP_SIZE;
                pa_program->program_state = 1;
            }
            else pa_program->program_state = 0;
            break;
        case L_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case LA_PCOMMAND:
            pa_program->register_memory[arg1] = arg2;
            pa_program->program_state = arg2;
            break;
        case LR_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg2];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case M_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] * pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case MR_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] * pa_program->register_memory[arg2];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case S_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] - pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case SR_PCOMMAND:
            pa_program->register_memory[arg1] = pa_program->register_memory[arg1] - pa_program->register_memory[arg2];
            pa_program->program_state = pa_program->register_memory[arg1];
            break;
        case ST_PCOMMAND:
            pa_program->memory[arg2+pa_program->register_memory[arg3]/4] = pa_program->register_memory[arg1];
            pa_program->program_state = pa_program->memory[arg2+pa_program->register_memory[arg3]/4];
            break;
    }
    pa_program->current_operation_address += OP_SIZE;
}



void run_program(pseudoassembly_program* pa_program){

    int ins_to_skip_log = 0;
    int operation_address_log = -1;
    get_input(pa_program);

    while(pa_program->current_operation_address < pa_program->memory_allocated + pa_program->operations_instructions_count * OP_SIZE){
        run_instruction(pa_program);
        log_program(pa_program, &ins_to_skip_log, &operation_address_log, 0);
    }
    log_program(pa_program, &ins_to_skip_log, &operation_address_log, 1);

    show_output(pa_program);
}

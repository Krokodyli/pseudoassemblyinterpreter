#ifndef PAP_H
#define PAP_H

#define OP_SIZE 4
#define MAX_INS_SIZE 3
#define OPERATIONS_INSTRUCTIONS_COUNT 18
#define MEMORY_INSTRUCTIONS_COUNT 2
#define REGISTER_SIZE 16

char MEMORY_INSTRUCTIONS[MEMORY_INSTRUCTIONS_COUNT][MAX_INS_SIZE];
char OPERATIONS_INSTRUCTIONS[OPERATIONS_INSTRUCTIONS_COUNT][MAX_INS_SIZE];

struct pseudoassembly_program {
    int memory_instructions_count;
    int memory_allocated;
    int* memory_addresses;
    char** memory_labels;

    int operations_instructions_count;
    int* operations_addresses;
    char** operations_labels;

    int* memory;

    int register_memory[REGISTER_SIZE];

    int current_operation_address;
    int last_operation_address;
    int program_state;
    
    char** code;

    int FLAG_NORMAL;
    int FLAG_SILENT;
    int FLAG_INTERVAL;
    int FLAG_LAST;
};
typedef struct pseudoassembly_program pseudoassembly_program;

#endif

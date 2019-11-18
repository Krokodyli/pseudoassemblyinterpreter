#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>

#include "lists.h"
#include "pai.h"

int parse_word(char** out, mString* mstr, int* mstr_size);

int parse_integer(int* out, mString* mstr, int* mstr_size);

void parse_label_and_operation_type(char cc, mString* mstr, int* mstr_size, char** label, int* op_inf, int* parsing_flag, int line);

void parse_memory_instruction(char cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, int* parsing_flag, int line);

void parse_register_register_operation(char cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, int* parsing_flag, int line);

void parse_register_memory_operation(int cc, mString* mstr, int* mstr_size, int* arg1_inf, int* arg2_inf, char** arg_label, int* parsing_flag, int line);

void parse_jump_instruction(char cc, mString* mstr, int* mstr_size, int* arg1_inf, char** arg_label, int* parsing_flag, int line);

void parse_line(FILE* file, char** line_str, char** label, int* op_inf, int* arg1_inf, int* arg2_inf, char** arg_label, int line);

pseudoassembly_program* parse_file(char* path);

#endif

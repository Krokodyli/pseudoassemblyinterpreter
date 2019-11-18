#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pap.h"
#include "pai.h"
#include "system_api.h"
#include "basicFunctions.h"

#define LOG_REGISTER_SIZE 16
#define LOG_ALWAYS_NEEDED_SIZE 10
#define COMMAND_BUFF_SIZE 256
#define INTEGER_WIDTH 11

#define LOG_SIZE_TIER1_CODE 1
#define LOG_SIZE_TIER1_REGISTER 3

#define LOG_SIZE_TIER2 8
#define LOG_SIZE_TIER2_CODE 2
#define LOG_SIZE_TIER2_REGISTER 3

#define LOG_SIZE_TIER3 10
#define LOG_SIZE_TIER3_CODE 2
#define LOG_SIZE_TIER3_REGISTER 2

#define LOG_SIZE_TIER4 12
#define LOG_SIZE_TIER4_CODE 3
#define LOG_SIZE_TIER4_REGISTER 2

#define LOG_SIZE_TIER5 14
#define LOG_SIZE_TIER5_CODE 4
#define LOG_SIZE_TIER5_REGISTER 2

#define LOG_SIZE_TIER6 16
#define LOG_SIZE_TIER6_CODE 5
#define LOG_SIZE_TIER6_REGISTER 2

void assert(int assertion, char* functionName);

void print_syntax_error(char* message, int line);

void assert_syntax_error(int assertion, char* message, int line);

void print_error(char* message);

void set_gui_size(int height, int variables_count, int* code_window, int* register_window, int* variables_window);

void log_operation(pseudoassembly_program* pa_program);

void log_program(pseudoassembly_program* pa_program, int* ins_to_skip, int* operation_address_log, int last);

int parse_command_argument(char buff[256], char label_buff[256], char arg_buff[256], int* offset);

void execute_command(pseudoassembly_program* pa_program, int* ins_to_skip, int* operation_address_to_log);

void parse_flags(pseudoassembly_program* pa_program, int argc, char* argv[]);

int get_integer(char* label, int offset);

void show_integer(char* label, int offset, int integer);

#endif

#ifndef PAI_H
#define PAI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pap.h"
#include "log.h"
#include "basicFunctions.h"

#define A_PCOMMAND 100
#define AR_PCOMMAND 118
#define C_PCOMMAND 300
#define CR_PCOMMAND 318
#define D_PCOMMAND 400
#define DR_PCOMMAND 418
#define J_PCOMMAND 1000
#define JN_PCOMMAND 1014
#define JP_PCOMMAND 1016
#define JZ_PCOMMAND 1026
#define L_PCOMMAND 1200
#define LA_PCOMMAND 1201
#define LR_PCOMMAND 1218
#define M_PCOMMAND 1300
#define MR_PCOMMAND 1318
#define S_PCOMMAND 1900
#define SR_PCOMMAND 1918
#define ST_PCOMMAND 1920

void free_program_memory();

void fill_variable_memory(int command, int arg1, int arg2, int addr_offset, int line, pseudoassembly_program* pa_program);

void fill_operation_memory(int command, int arg1, int arg2, char* arg_label, int addr_offset, int line, pseudoassembly_program* pa_program);

void get_input(pseudoassembly_program* pa_program);

void show_output(pseudoassembly_program* pa_program);

void run_instruction(pseudoassembly_program* pa_program);

void run_program(pseudoassembly_program* pa_program);

#endif

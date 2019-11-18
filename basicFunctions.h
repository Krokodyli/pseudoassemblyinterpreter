#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists.h"
#include "log.h"

int hash_instruction(char* word);

int binsearch(int* array, int size, int v);

int char_array_to_int(int* out, char* str);

int check_if_char_array_is_connected(char* str);

int str_cmp(char* ch1, char* ch2);

void fix_char_array(char** arr);

int is_variable_input(char* char_array);

int is_variable_output(char* char_array);

#endif

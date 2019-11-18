#ifndef LISTS_H
#define LISTS_H

#include <stdio.h>
#include <stdlib.h>

#include "basicFunctions.h"

struct mString_node {
    char c;
    struct mString_node* next_node;
};
typedef struct mString_node* mString;

void mString_add(char c, mString* mstr);

void mString_to_char_array(mString* mstr, int size, char** char_array);

void mString_clear(mString* mstr);

int mString_parse_int(int* out, mString mstr);


struct instruction_list_node {
    char* line_str;
    char* label;
    char* arg_label;
    int command;
    int arg1;
    int arg2;
    int line;
    struct instruction_list_node* next_node;
};
typedef struct instruction_list_node* instruction_list;

void instruction_list_add(char* line_str, char* label, int command, int arg1, int arg2, char* arg_label, int line, instruction_list* ins_list);

#endif

#include "lists.h"

void mString_add(char c, mString* mstr){
    mString newHead = (mString)malloc(sizeof(struct mString_node));
    assert(newHead != NULL, "mString_add");
    newHead->c = c;
    newHead->next_node = *mstr;
    (*mstr) = newHead;
}

void mString_to_char_array(mString* mstr, int size, char** char_array){
    int i;
    mString oldHead;
   (*char_array) = (char*)malloc(sizeof(char)*(size+1));
   (*char_array)[size] = '\0';
   for(i = size - 1; i >= 0; i--){
        oldHead = *mstr;
        (*char_array)[i] = (*mstr)->c;
        (*mstr) = (*mstr)->next_node;
        free(oldHead);
   }
   (*mstr) = NULL;
}

void mString_clear(mString* mstr){
    mString temp;
    mString head = *mstr;
    while(head != NULL){
        temp = head;
        head = head->next_node;
        free(temp);
    }
    (*mstr) = NULL;
}

int mString_parse_int(int* out, mString mstr){
    mString head = mstr;
    int number = 0;
    int multiplier = 1;
    int started_word_flag = 0;
    int sign = 1;
    while(head != NULL){
        if(head->c != ' ' && started_word_flag < 2){
            if(head->c == '-'){
                sign = 0;
                started_word_flag = 2;
            }
            else{
                started_word_flag = 1;
                if(number > number+(head->c - '0')*multiplier || head->c < '0' || head->c > '9'){
                    *out = 0;
                    return 0;
                }
                number += (head->c - '0') * multiplier;
                multiplier *= 10;
            }
        }
        else if(head->c != ' ' && started_word_flag == 2){
            return 0;
        }
        else if(head->c == ' ' && started_word_flag == 1){
            started_word_flag = 2;
        }
        head = head->next_node;
    }
    *out = number * sign;
    return 1;
}


void instruction_list_add(char* line_str, char* label, int command, int arg1, int arg2, char* arg_label, int line, instruction_list* ins_list){
    instruction_list newHead = (instruction_list)malloc(sizeof(struct instruction_list_node));
    assert(newHead != NULL, "instruction_list_add");
    newHead->line_str = line_str;
    newHead->label = label;
    newHead->command = command;
    newHead->arg1 = arg1;
    newHead->arg2 = arg2;
    newHead->arg_label = arg_label;
    newHead->line = line;
    newHead->next_node = *ins_list;
    (*ins_list) = newHead;
}


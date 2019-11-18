#include "basicFunctions.h"
#include "system_api.h"

int hash_instruction(char* word){
    assert(word[0] >= 'A' && word[0] <= 'Z', "hash_instruction");
    assert((word[1] >= 'A' && word[1] <= 'Z') || word[1] == '\0', "hash_instruction");
    if(word[1] == '\0')
        return (word[0] - 'A' + 1) * 100;
    return (word[0] - 'A' + 1) * 100 + word[1] - 'A' + 1;
}


int binsearch(int* array, int size, int v){
    int l = 0, r = size, s;
    while(l+1 < r){
        s = (l+r)/2;
        if(array[s] <= v) l = s;
        else r = s;
    }
    return l;
}

int char_array_to_int(int* out, char* str){
    int number = 0;
    int sign = 1;
    int it = 0;
    while(str[it] != '\0'){
        if(it == 0 && str[it] == '-') sign = -1;
        else if(str[it] < '0' || str[it] > '9'){
            return 0;
        }
        if(number * 10 < number){
            return 0;
        }
        number += str[it] - '0';
        number *= 10;
        it++;
    }
    number /= 10;
    *out = number * sign;
    return 1;
}

int check_if_char_array_is_connected(char* str){
    int it = 0;
    int flag = 0;
    while(str[it] != '\0'){
        if(str[it] == ' '){
            if(flag == 1)
                flag = 2;
        }
        else if(flag == 0){
            flag = 1;
        }
        else if(flag == 2)
            return 0;
        it++;
    }
    return 1;
}

int str_cmp(char* ch1, char* ch2){
    int i;
    int it1 = 0, it2 = 0;
    int f1=-1, l1=-1, f2=-1, l2=-1;
    while(ch1[it1] != '\0'){
        if(ch1[it1] != ' '){
            if(f1 == -1) f1 = it1;
            l1 = it1;
        }
        it1++;
    }
    while(ch2[it2] != '\0'){
        if(ch2[it2] != ' '){
            if(f2 == -1) f2 = it2;
            l2 = it2;
        }
        it2++;
    }
    assert(f1 != -1 && f2 != -1, "str_cmp");
    if(l1 - f1 != l2 - f2) return 1;
    for(i = 0; i <= (l1-f1); i++){
        if(ch1[f1+i] != ch2[f2+i])
            return 1;

    }
    return 0;
}

void fix_char_array(char** arr){
    int firstChar = -1;
    int lastChar = -1;
    int it = 0, i;
    char* newArray;
    if(*arr == NULL)
        return;
    while((*arr)[it] != '\0'){
        if((*arr)[it] != ' '){
            if(firstChar == -1) firstChar = it;
            lastChar = it;
        }
        it++;
    }
    if(firstChar == -1){
        free(*arr);
        *arr = NULL;
        return;
    }
    newArray = (char*)malloc(sizeof(char)*(lastChar-firstChar+2));
    for(i = 0; i < lastChar-firstChar+1; i++){
        newArray[i] = (*arr)[firstChar+i];
    }
    newArray[lastChar-firstChar+1] = '\0';
    free(*arr);
    *arr = newArray;
}

int is_variable_input(char* char_array){
    int len = strlen(char_array);
    if(len < 4) return 0;
    return (char_array[len-1] == 'N' && char_array[len-2] == 'I' && char_array[len-3] == '_');
}

int is_variable_output(char* char_array){
    int len = strlen(char_array);
    if(len < 5) return 0;
    return (char_array[len-1] == 'T' && char_array[len-2] == 'U' && char_array[len-3] == 'O' && char_array[len-4] == '_');
}

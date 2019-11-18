#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system_api.h"
#include "basicFunctions.h"
#include "lists.h"
#include "parse.h"
#include "pai.h"

int main(int argc, char *argv[]){
    pseudoassembly_program* pap;
    setup_console_colors();
    if(argc < 2){
        print_error("No input file");
    }
    pap = parse_file(argv[argc-1]);
    parse_flags(pap, argc, argv);
    run_program(pap);
    restore_console_colors();
    return 0;
}

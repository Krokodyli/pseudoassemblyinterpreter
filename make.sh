#!/bin/bash

gcc -Wall -ansi -pedantic -Werror system_api.c basicFunctions.c log.c lists.c pai.c parse.c main.c -o pai

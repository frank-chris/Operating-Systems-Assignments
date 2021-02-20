#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell.h"

int make_directory(char ** arguments){
    /*
     function to implement mkdir
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "mkdir: missing operand\n");
    }
    else{
        int i = 1;
        // loop through all the directory names given by the user
        while(arguments[i] != NULL){
            struct stat st = {0};

            // create the directory if it doesn't exist already
            if (stat(arguments[i], &st) == -1) {
                mkdir(arguments[i], 0777);
            }
            i++;
        }
    }
    return 1;
}
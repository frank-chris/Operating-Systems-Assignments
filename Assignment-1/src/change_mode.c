#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "shell.h"

int change_mode(char ** arguments){
    /*
     function to implement chmod
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "chmod: missing operand\n");
    }
    else if(arguments[2] == NULL){
        fprintf(stderr, "chmod: missing operand after ‘%s’\n", arguments[1]);
    }
    else{
        char *ptr;
        long mode;

        // convert the argument given by user to octal 
        mode = strtol(arguments[1], &ptr, 8);

        // change mode for all files mentioned by user
        int i = 2;
        while(arguments[i] != NULL){
            // changing mode of file arguments[i]
            if (chmod(arguments[i], mode) < 0){
                // error handling
                fprintf(stderr, "chmod error\n");
            }
            i++;
        }
    }
    return 1;
}
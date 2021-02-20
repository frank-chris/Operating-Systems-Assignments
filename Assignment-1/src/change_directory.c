#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int change_directory(char ** arguments){
    /*
     function to implement cd
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "cd: expected an argument\n");
    }
    else{
        // change directory
        if (chdir(arguments[1]) != 0) {
            perror("chdir error");
        }
    }
    return 1;
}
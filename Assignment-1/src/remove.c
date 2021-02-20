#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int remove_f(char ** arguments){
    /*
     function that implements rm
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "rm: missing operand\n");
    }
    else if(strcmp(arguments[1], "-r") == 0){
        // with -r option

        int i = 2;

        if(arguments[2] == NULL){
            fprintf(stderr, "rm: missing operand\n");
            return 1;
        }

        // loop though all directories given by user
        while(arguments[i] != NULL){
            // remove_tree(path) recursively deletes the directory passed to it
            remove_filetree(arguments[i]);
            i++;
        }
    }
    else if(strcmp(arguments[1], "-d") == 0){
        // -d option
        int i = 2;

        // loops through all directories given by user and 
        // removes the directory if it is empty
        while(arguments[i] != NULL){
            if(rmdir(arguments[i]) > 0){
                fprintf(stderr, "remove error: couldn't remove directory\n");
            }
            i++;
        }
    }
    else{
        int i = 1;

        // loops through all files given by the user and removes them
        while(arguments[i] != NULL){
            if(remove(arguments[i])){
                fprintf(stderr, "remove error: couldn't remove file\n");
            }
            i++;
        }
    }
    return 1;
}
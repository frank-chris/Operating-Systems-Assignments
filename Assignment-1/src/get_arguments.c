#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

char ** get_arguments(char * command){
    /*
     function to split the command input into arguments
     arguments: the command read as input
     return: the arguments of the command after splitting
    */
    int index = 0;
    int argument_count = NO_OF_ARGUMENTS;
    char * argument;
    char ** arguments = malloc(sizeof(char *)*argument_count);

    if(arguments == NULL){
        fprintf(stderr, "Allocation Error: Error in allocating memory to parse input command.\n");
        exit(EXIT_FAILURE);
    }

    // first argument (command tokenized using DELIMITER)
    argument = strtok(command, DELIMITER);

    while(argument != NULL){
        arguments[index++] = argument;

        // subsequent arguments being tokenized
        argument = strtok(NULL, DELIMITER);

        // allocate more memory if previously allocated is not enough
        if(index >= argument_count){
            argument_count += NO_OF_ARGUMENTS;
            arguments = realloc(arguments, sizeof(char *)*argument_count);

            if(arguments == NULL){
                fprintf(stderr, "Allocation Error: Error in allocating memory to parse input command.\n");
                exit(EXIT_FAILURE);
            }            
        }
    }
    // last index set as NULL
    arguments[index] = NULL;
    
    // return list of arguments after splitting them
    return arguments;
}
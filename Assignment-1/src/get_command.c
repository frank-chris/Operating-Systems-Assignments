#include <stdio.h> 
#include <stdlib.h> 
#include "shell.h"

char * get_command(){
    /*
     function to read the input command
     arguments: none
     return: the line read from user
    */
    int index = 0;
    int command_length = COMMAND_LENGTH;
    char character;
    char * command = malloc(sizeof(char)*command_length);
    
    if(command == NULL){
        fprintf(stderr, "Allocation Error: Error in allocating memory to read input command.\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        // read character by character
        character = fgetc(stdin);

        // store the character read in char * command
        if(character != '\n' && character != EOF){
            command[index++] = character;
        }
        // replace \n and EOF with \0
        else{
            command[index++] = '\0';
            return command;
        }
        
        // allocate more memory if previously allocated is not enough
        if(index >= command_length){
            command_length += COMMAND_LENGTH;
            command = realloc(command, sizeof(char)*command_length);
            if(command == NULL){
                fprintf(stderr, "Allocation Error: Error in allocating memory to read input command.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
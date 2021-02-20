#include <string.h>
#include <stdio.h>
#include "shell.h"

char ** join_spaces(char ** arguments){
    /*
     function to join together arguments ending with \ with 
     the next argument using space
     arguments: list of arguments after splitting the command
     return: list of arguments after joining together arguments 
     ending with \ with the next argument using space
    */

    int i = 0;

    while(arguments[i+1] != NULL && arguments[i] != NULL){
        if(arguments[i][strlen(arguments[i])-1] == '\\'){
            
            // if an argument ends with \, replace it with \0
            arguments[i][strlen(arguments[i])-1] = '\0';

            // join it with the next argument
            sprintf(arguments[i], "%s %s", arguments[i], arguments[i+1]);
            int j = i + 1;

            // shift all other arguments forward
            while(arguments[j+1] != NULL){
                strcpy(arguments[j], arguments[j+1]);
                j++;
            }
            // make last argument NULL
            arguments[j] = NULL;
        }
        i++;
    }
    return arguments;
}
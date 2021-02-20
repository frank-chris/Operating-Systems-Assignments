#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int parse_command(char ** arguments){
    /*
     function to parse the arguments to check for & (background process)
     and to execute the command in the background if needed
     arguments: list of arguments after splitting the command
     return: value returned by determine_command_type() 
             or 0 in child process and 1 in parent process if running a command in background
    */
    int i = 0;
    char last_character;

    while(arguments[i] != NULL){
        i++;
    }
    // last character of the command
    last_character = arguments[i-1][strlen(arguments[i-1])-1];

    if(last_character == '&'){
        // execute command in background

        int rc = fork();

        if(rc < 0){
            fprintf(stderr,"Fork unsuccessful(while trying to run as background process)\n");
            return 1;
        }
        else if(rc == 0){
            // child process

            // change the process group id of child process
            // to remove it from foreground group
            setpgid(getpid(), 0);

            arguments[i-1] = NULL;

            // determines command and executes in the background
            determine_command_type(arguments);

            // exits after executing command in background
            return 0;
        }
        else{
            // parent process, returns control immediately
            return 1;
        }
    }
    else{
        // execute the command normally if there is no & in the arguments
        return determine_command_type(arguments);
    }
}
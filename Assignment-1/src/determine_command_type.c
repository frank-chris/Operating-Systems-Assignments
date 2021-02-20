#include <stdio.h>
#include <string.h>
#include "shell.h"

int determine_command_type(char ** arguments){
    /*
     function to decide which command to execute, ie. which function to call
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if (arguments[0] == NULL) {
        return 1;
    }
    else if(strcmp(arguments[0], "ls") == 0) {
        return list_directory(arguments);
    }
    else if(strcmp(arguments[0], "grep") == 0) {
        return grep(arguments);
    }
    else if(strcmp(arguments[0], "cat") == 0) {
        return cat(arguments);
    }
    else if(strcmp(arguments[0], "rm") == 0) {
        return remove_f(arguments);
    }
    else if(strcmp(arguments[0], "mv") == 0) {
        return move(arguments);
    }
    else if(strcmp(arguments[0], "cp") == 0) {
        return copy(arguments);
    }
    else if(strcmp(arguments[0], "cd") == 0) {
        return change_directory(arguments);
    }
    else if(strcmp(arguments[0], "pwd") == 0) {
        return current_working_directory(arguments);
    }
    else if(strcmp(arguments[0], "chmod") == 0) {
        return change_mode(arguments);
    }
    else if(strcmp(arguments[0], "mkdir") == 0) {
        return make_directory(arguments);
    }
    else if(strcmp(arguments[0], "exit") == 0) {
        return exit_shell();
    }
    else{
        // if the command is not implemented in this shell,
        // then run the inbuilt binary
        return execute(arguments);
    }
    return 1;
}
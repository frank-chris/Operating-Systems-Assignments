#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

int current_working_directory(char ** arguments){
    /*
     function to implement pwd
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    char directory[256];

    // get the current working directory
    if (getcwd(directory, sizeof(directory)) == NULL) {
        // error
        perror("getcwd error");
    }
    else {
        // successful
        printf("%s\n", directory);
    }
    return 1;
}
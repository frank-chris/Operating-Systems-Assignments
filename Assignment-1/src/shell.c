#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"

int main(){
    int status = 1;
    char * command;
    char ** arguments;
    char current_directory[256];

    printf("\e[1;1H\e[2J"); // clears the terminal

    printf("\nWelcome to a simple shell\nby Chris Francis\n\n");

    // to display the current working directory in the prompt
    if (getcwd(current_directory, sizeof(current_directory)) == NULL) {
        perror("getcwd error");
    }

    // loop runs until status is set to 0
    do {
        // find current working directory again if last command was cd
        if(strcmp(arguments[0], "cd") == 0) {
            if (getcwd(current_directory, sizeof(current_directory)) == NULL) {
                perror("getcwd error");
            }
        }
        printf("%s >> ", current_directory);   

        // read the command input
        command = get_command();

        // split the command into arguments
        arguments = join_spaces(get_arguments(command));

        // parse arguments to check for & (background process)
        status = parse_command(arguments);    

    } while (status);
    

    return EXIT_SUCCESS;
}
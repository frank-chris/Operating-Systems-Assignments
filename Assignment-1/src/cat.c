#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"

int cat(char ** arguments){
    /*
     function to implement cat
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    int fd, buf; 
    int i = 1;

    if(arguments[1] == NULL){
        fprintf(stderr, "cat: expected an argument\n");
    }
    else{
        while(arguments[i] != NULL){

            // open file in read-only mode
            fd = open(arguments[i], O_RDONLY);
            
            // error handling
            if(fd < 0) { 
                perror("cat: cannot open file"); 
                return 1;
            }

            // print filename
            printf("\n%s:\n\n", arguments[i]);

            // print file contents
            while(read(fd, &buf, 1)){
                write(STDOUT_FILENO, &buf, 1);
            }
            printf("\n");

            // close file
            close(fd);

            i++;
        }
    }
    return 1;
}
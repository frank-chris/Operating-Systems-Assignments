#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int execute(char ** arguments){
    /*
     function to execute inbuilt binaries
     arguments: list of arguments after splitting the command
     return: 1 by parent (to make sure the loop in main() continues)
             0 by child  (to terminate the child process after the binary is run)
    */
    int rc = fork();
    
    if(rc < 0){
        // fork unsuccessful
        fprintf(stderr,"Fork unsuccessful(while trying to execute inbuilt binary)\n");
        return 1;
    }
    else if(rc == 0){
        // child process
        execvp(arguments[0], arguments);
        return 0;
    }
    else{
        // parent process
        wait(NULL);
        return 1;
    }
}
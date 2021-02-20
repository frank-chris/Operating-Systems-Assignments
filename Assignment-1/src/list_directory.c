#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "shell.h"

int list_directory(char ** arguments){
    /*
     function to implement ls
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        // if no other arguments with ls

        // open current directory
        DIR * dp = opendir(".");
        
        // check if directory was opened
        if(dp == NULL){
            perror("opendir error");
        }

        // loop through the entries of the current directory
        struct dirent *d;
        while ((d = readdir(dp)) != NULL) {
            // ignore if entry is . or ..
            if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")){
                continue;
            }
            printf("%s\n", d->d_name);
        }
        // close directory
        closedir(dp);
    }
    else{
        // there are arguments given to ls
        int i = 1;

        // loop through all the arguments and do ls on each argument
        while(arguments[i] != NULL){
            // open directory
            DIR * dp = opendir(arguments[i]);
        
            // check if directory was opened
            if(dp == NULL){
                perror("opendir error");
            }

            // print directory name
            printf("\n%s:\n", arguments[i]);

            struct dirent *d;

            // read the entries of the directory
            while ((d = readdir(dp)) != NULL) {
                // ignore if entry is . or ..
                if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")){
                    continue;
                }
                printf("%s\n", d->d_name);
            }
            // close the directory
            closedir(dp);

            i++;
        }
    }
    return 1;
}
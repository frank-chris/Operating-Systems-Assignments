#include <stdlib.h>
#include <stdio.h> 
#include <dirent.h>
#include <string.h>
#include "shell.h"

int grep(char ** arguments){   
    /*
     function to implement grep
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    char * temp;
    size_t buf = 0;

    int i = 2;

    // incorrect usage
    if(arguments[1] == NULL){
        fprintf(stderr, "grep: searchterm [file ...]\n");
    }
    else if(arguments[2] == NULL){
        fprintf(stderr, "grep: searchterm [file ...]\n");
    }
    else{
        FILE *fp;
        
        // open each file given by user and search for the pattern 
        while(arguments[i] != NULL){
            // open file
            fp = fopen(arguments[i], "r");

            // print file name
            printf("\n%s:\n", arguments[i]);

            // check if file was opened
            if(fp == NULL){
                printf("grep: cannot open file\n");
                return 1;
            }
            // read each line from the file
            while (getline(&temp, &buf, fp) != -1)
            {   // print the line if it contains the pattern
                if (strstr(temp, arguments[1]) != NULL){
                    printf("%s", temp);
                }     
            }
            printf("\n");
            // close file
            fclose(fp);
            i++;
        }
    }
    return 1;
}
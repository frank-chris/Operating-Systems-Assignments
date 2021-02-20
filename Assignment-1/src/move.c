#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>
#include "shell.h"

int move(char ** arguments){
    /*
     function to implement mv
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "mv: missing file operand\n");
    }
    else if(arguments[2] == NULL){
        fprintf(stderr, "mv: missing destination file operand after '%s'\n", arguments[1]);
    }
    else if(arguments[3] == NULL){
        // when there are 2 arguments

        struct stat stat_source, stat_destination;

        // stat() on source
        if (stat(arguments[1], &stat_source) == -1) { 
            perror("stat");
            return 1;
        }
        // check if source is a directory
        if (S_ISDIR(stat_source.st_mode) != 0) {
            fprintf(stderr, "mv: first argument cannot be a directory\n");
            return 1;
        }

        // stat() destination
        if (stat(arguments[2], &stat_destination) == -1) { 
            // rename source to destination if destination doesn't exist
            // because that's when stat() returns -1
            if (rename(arguments[1], arguments[2])){
                fprintf(stderr, "mv: rename failed\n");
            }
            return 1;
        }

        // if destination is not a directory
        if (S_ISDIR(stat_destination.st_mode) == 0) {
            if (rename(arguments[1], arguments[2])){
                fprintf(stderr, "mv: rename failed\n");
            }
        }
        else{
            // if destination is a directory
            char * filepath;

            // create the destination file path
            filepath = malloc((strlen(arguments[2]) + strlen(basename(arguments[1])) + 1)*sizeof(char));
            strcpy(filepath, arguments[2]);
            strcat(filepath, "/");
            strcat(filepath, basename(arguments[1]));

            // rename source to destination 
            if (rename(arguments[1], filepath)){
                fprintf(stderr, "mv: rename failed\n");
            }
        }
    }
    else{
        // moving multiple files into destination directory
        int n = 3;
        while(arguments[n] != NULL){
            n++;
        }

        // index of the destination in arguments
        n = n - 1;

        struct stat stat_destination;

        // stat() on destination
        if (stat(arguments[n], &stat_destination) == -1) { 
            perror("stat");
            return 1;
        }
        // check if destination is a directory, exit function if not
        if (S_ISDIR(stat_destination.st_mode) == 0) {
            fprintf(stderr, "mv: target '%s' is not a directory\n", arguments[n]);
            return 1;
        }

        // looping through all the files and moving them to destination directory
        for(int i = 1; i < n; ++i){

            // create filepath of destination file
            char * filepath;
            filepath = malloc((strlen(arguments[n]) + strlen(basename(arguments[i])) + 1)*sizeof(char));
            strcpy(filepath, arguments[n]);
            strcat(filepath, "/");
            strcat(filepath, basename(arguments[i]));

            // move the file to the destination directory
            if (rename(arguments[i], filepath)){
                fprintf(stderr, "mv: rename failed\n");
            }
        }
    }
    return 1;
}
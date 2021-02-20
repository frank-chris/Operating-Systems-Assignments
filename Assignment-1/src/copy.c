#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include "shell.h"

int copy(char ** arguments){
    /*
     function to implement cp
     arguments: list of arguments after splitting the command
     return: 1 (to make sure the loop in main() continues)
    */
    if(arguments[1] == NULL){
        fprintf(stderr, "cp: missing file operand\n");
    }
    else if(strcmp(arguments[1], "-r") == 0){
        // directory to directory copying
        if(arguments[2] == NULL){
            fprintf(stderr, "cp: missing file operand\n");
        }
        else if(arguments[3] == NULL){
            fprintf(stderr, "cp: missing destination file operand after '%s'\n", arguments[2]);
        }
        else{
            DIR * dp;
            struct stat stat_source, stat_destination, stat_entry;
            struct dirent * entry;
            char * dirpath;
            char * sourcepath;

            // stat() call on source
            if (stat(arguments[2], &stat_source) == -1) { 
                perror("stat");
                return 1;
            }
            // checking if source is a directory
            if (S_ISDIR(stat_source.st_mode) == 0) {
                fprintf(stderr, "cp: argument is not a directory\n");
                return 1;
            }

            // stat() call on destination
            if (stat(arguments[3], &stat_destination) == -1) { 
                perror("stat");
                return 1;
            }
            // checking if destination is a directory
            if (S_ISDIR(stat_destination.st_mode) == 0) {
                fprintf(stderr, "cp: argument is not a directory\n");
                return 1;
            }

            // opening source directory
            if ((dp = opendir(arguments[2])) == NULL) {
                fprintf(stderr, "opendir error");
                return 1;
            }

            // reading the directory entries of source
            while ((entry = readdir(dp)) != NULL) {
                
                // ignore . and ..
                if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                    continue;

                // creating the path to which the entries of source have to be copied
                dirpath = malloc((strlen(arguments[3]) + strlen(basename(arguments[2])) + strlen(entry->d_name) + 2)*sizeof(char));
                strcpy(dirpath, arguments[3]);
                strcat(dirpath, "/");
                strcat(dirpath, basename(arguments[2]));

                struct stat st = {0};
                // making a directory with the same name as source directory
                // inside the destination directory if it doesn't already exist
                if (stat(dirpath, &st) == -1) {
                    mkdir(dirpath, 0777);
                }
                // creating the remainder of the path
                strcat(dirpath, "/");
                strcat(dirpath, entry->d_name);

                // creating the path of the entry in source directory which is to be copied
                sourcepath = malloc((strlen(arguments[2]) + strlen(entry->d_name) + 1)*sizeof(char));
                strcpy(sourcepath, arguments[2]);
                strcat(sourcepath, "/");
                strcat(sourcepath, entry->d_name);

                // stat() on the entry in the source directory which is to be copied
                if (stat(sourcepath, &stat_entry) == -1) { 
                    perror("stat");
                    return 1;
                }

                // if the the entry is not a directory, copy it to destination directory
                if (S_ISDIR(stat_entry.st_mode) == 0) {
                    // open source file
                    int fd1 = open(sourcepath, O_RDONLY);

                    // open source file, create if doesn't exist
                    int fd2 = open(dirpath, O_WRONLY | O_CREAT, 0777);
                    int buf;
                    
                    // check if files were opened
                    if(fd1 < 0) { 
                        perror("open error"); 
                        return 1;
                    }
                    if(fd2 < 0) { 
                        perror("open error"); 
                        return 1;
                    }

                    // reading contents of source file and writing it into destination file
                    while(read(fd1, &buf, 1)){
                        write(fd2, &buf, 1);
                    }

                    // close files
                    close(fd1);
                    close(fd2);
                }
            }
            free(sourcepath);
            free(dirpath);
        }
    }
    else if(arguments[2] == NULL){
        fprintf(stderr, "cp: missing destination file operand after '%s'\n", arguments[1]);
    }
    else if(arguments[3] == NULL){
        // copying file to file

        // open source file
        int fd1 = open(arguments[1], O_RDONLY);
        
        // open destination file
        int fd2 = open(arguments[2], O_WRONLY | O_CREAT, 0777);
        int buf;
        
        // check if files were opened
        if(fd1 < 0) { 
            perror("open error"); 
            return 1;
        }
        if(fd2 < 0) { 
            perror("open error"); 
            return 1;
        }

        // reading contents of source file and writing it into destination file
        while(read(fd1, &buf, 1)){
            write(fd2, &buf, 1);
        }
        
        // close files
        close(fd1);
        close(fd2);
    }
    else{
        // copying multiple files into a directory

        int n = 3;
        while(arguments[n] != NULL){
            n++;
        }

        // n is the index of the last argument in arguments
        n = n - 1;

        struct stat stat_destination;
        
        // stat() on destination
        if (stat(arguments[n], &stat_destination) == -1) { 
            perror("stat");
            return 1;
        }
        // check if destination is a directory
        if (S_ISDIR(stat_destination.st_mode) == 0) {
            fprintf(stderr, "cp: target '%s' is not a directory\n", arguments[n]);
            return 1;
        }

        // loop to copy files into the destination directory
        for(int i = 1; i < n; ++i){

            // open source file
            int fd1 = open(arguments[i], O_RDONLY);

            // create path of destination file
            char * dirpath;
            dirpath = malloc((strlen(basename(arguments[i])) + strlen(arguments[n]) + 1)*sizeof(char));
            strcpy(dirpath, arguments[n]);
            strcat(dirpath, "/");
            strcat(dirpath, basename(arguments[i]));

            // open destination file, create if it doesn't exist
            int fd2 = open(dirpath, O_WRONLY | O_CREAT, 0777);
            int buf;
            
            // check if files were opened successfully
            if(fd1 < 0) { 
                perror("open error"); 
                return 1;
            }
            if(fd2 < 0) { 
                perror("open error"); 
                return 1;
            }

            // reading contents of source file and writing it into destination file
            while(read(fd1, &buf, 1)){
                write(fd2, &buf, 1);
            }

            // close files
            close(fd1);
            close(fd2);

            free(dirpath);
        }
    }
    return 1;
}
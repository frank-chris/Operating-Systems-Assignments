#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shell.h"

void remove_filetree(const char * filepath){
    /*
     recursive function that deletes the directory given by filepath
     arguments: path to the directory to be deleted recursively
     return: none
    */
    char * entrypath;
    DIR * dp;
    struct stat stat_path, stat_entry;
    struct dirent * entry;
    
    // stat() on the given path
    if (stat(filepath, &stat_path) == -1) { 
        perror("stat");
        exit(EXIT_FAILURE);
    }
    // check if given path is of a directory
    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "rm: argument is not a directory");
        exit(EXIT_FAILURE);
    }

    // open the directory
    if ((dp = opendir(filepath)) == NULL) {
        fprintf(stderr, "opendir error");
        exit(EXIT_FAILURE);
    }

    // loop through the entries of the directory
    while ((entry = readdir(dp)) != NULL) {
        // ignore . and ..
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // create the path of an entry in the directory
        entrypath = malloc((strlen(filepath) + strlen(entry->d_name) + 1)*sizeof(char));
        strcpy(entrypath, filepath);
        strcat(entrypath, "/");
        strcat(entrypath, entry->d_name);

        // stat() on the entry
        if (stat(entrypath, &stat_entry) == -1) { 
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // check if the entry is a directory
        // call this function recursively if it is a directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            remove_filetree(entrypath);
            continue;
        }

        // if the entry is not a directory, then unlink it
        if (unlink(entrypath) != 0){
            fprintf(stderr, "remove error: couldn't remove a file");
        }
        free(entrypath);
    }
    // delete the directory which is now empty
    if (rmdir(filepath) != 0){
        fprintf(stderr, "remove error: couldn't remove a directory");
    }
    closedir(dp);
}
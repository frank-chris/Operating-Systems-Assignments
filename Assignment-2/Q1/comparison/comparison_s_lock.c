#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>     // for printf(), stderr
#include <stdlib.h>    // for malloc(), EXIT_FAILURE, exit()
#include <string.h>    // for strcpy()
#include <sys/time.h>  // for gettimeofday(), struct timeval
#include <math.h>      // for ceil(), floor()
#include "trie.h"

// global trie variable
trie_t trie;

// global array of words that will be used in the workload operations
char** query_set;

void swap(char* x, char* y){
    /*
        Function to swap two char variables

            Parameters:
                x(char*): the first char variable
                y(char*): the second char variable

            Returns:
                none
    */
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char* a, int l, int r, int* count){
    /*
        Recursive function to create all permutations of a given string

            Parameters:
                a(char*): the given string
                l(int): lowest index of the string
                r(int): highest index of the string
                count(int*): count variable to keep track of permutations created so far

            Returns:
                none
    */
    int i;
    if(l == r){
        char* temp = (char*)malloc(sizeof(char)*10);
        strcpy(temp, a);
        query_set[*count] = temp;
        *count += 1;
    }
    else{
        for(i = l; i <= r; i++){
            swap((a+l), (a+i));
            permute(a, l+1, r, count);
            swap((a+l), (a+i));
        }
    }
}

void* write_intensive(void* arg){
    /*
        Function to perform write_intensive(80-20 write-read) operations

            Parameters:
                arg(void*): the start and end indices of the global array query_set
                            to be used to perform operations on the trie

            Returns:
                none
    */
    int start = *((int*)arg);
    int end = *((int*)arg+1);
    int end_find = (int)floor(start + ((end-start)/4));

    for(int i = start; i <= end; i++){
        insert(trie, query_set[i], i);
    }

    int value;

    for(int i = start; i <= end_find; i++){
        find(trie, query_set[i], &value);
    }
    return NULL;
}

void* read_intensive(void* arg){
    /*
        Function to perform read_intensive(20-80 write-read) operations

            Parameters:
                arg(void*): the start and end indices of the global array query_set
                            to be used to perform operations on the trie

            Returns:
                none
    */
    int start = *((int*)arg);
    int end = *((int*)arg+1);
    int end_insert = (int)floor(start + ((end-start)/4));

    int value;

    for(int i = start; i <= end; i++){
        find(trie, query_set[i], &value);
    }

    for(int i = start; i <= end_insert; i++){
        insert(trie, query_set[i], i);
    }
    return NULL;
}

void* read_write(void* arg){
    /*
        Function to perform mixed read-write(50-50 write-read) operations

            Parameters:
                arg(void*): the start and end indices of the global array query_set
                            to be used to perform operations on the trie

            Returns:
                none
    */
    int start = *((int*)arg);
    int end = *((int*)arg+1);
    
    for(int i = start; i <= end; i++){
        insert(trie, query_set[i], i);
    }

    int value;

    for(int i = start; i <= end; i++){
        find(trie, query_set[i], &value);
    }
    return NULL;
}

long* run_workloads(int thread_count){
    /*
        Function to create threads, divide work and calculate time taken for each workload

            Parameters:
                thread_count(int): number of threads

            Returns:
                times(long array): times taken for each workload(in microseconds)
    */

    static long times[3] = {0, 0, 0};
    struct timeval start, end;

    // create the list of start and end indices to be passed to threads
    int indices[2*thread_count];

    for(int i = 0; i < thread_count; i++){
        indices[2*i] = i*(3628800/thread_count);
        indices[(2*i)+1] = (i == thread_count-1)?(3628799):((i+1)*(3628800/thread_count) - 1);
    }

    // running write intensive workload
    pthread_t w_threads[thread_count];

    gettimeofday(&start, NULL);

    for(int i = 0; i < thread_count; i++){
        pthread_create(&w_threads[i], NULL, &write_intensive, &indices[2*i]);
    }

    for(int i = 0; i < thread_count; i++){
        pthread_join(w_threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    times[0] = ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec));

    // running read intensive workload
    pthread_t r_threads[thread_count];

    gettimeofday(&start, NULL);

    for(int i = 0; i < thread_count; i++){
        pthread_create(&r_threads[i], NULL, &read_intensive, &indices[2*i]);
    }

    for(int i = 0; i < thread_count; i++){
        pthread_join(r_threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    times[1] = ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec));

    // running mixed read-write workload
    pthread_t rw_threads[thread_count];

    gettimeofday(&start, NULL);

    for(int i = 0; i < thread_count; i++){
        pthread_create(&rw_threads[i], NULL, &read_write, &indices[2*i]);
    }

    for(int i = 0; i < thread_count; i++){
        pthread_join(rw_threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    times[2] = ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec));

    return times;
}

int main(int argc, char const *argv[]){
    if(argc != 2){ // incorrect number of arguments
        printf("\nIncorrect usage. Provide max_thread_count(space-separated).\n");
        exit(EXIT_FAILURE);
    }

    // maximum number of threads to be considered
    int max_thread_count = atoi(argv[1]);
    
    // initialise trie
    trie = init_trie();

    int query_count = 0;

    query_set = (char**)malloc(sizeof(char*)*3628800);

    char start_query[10] = "abcdefghij";

    // fill query_set with all permutations of start_query
    permute(start_query, 0, 9, &query_count);

    FILE* fp;

    // open file
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            fp = fopen("comparison/s_lock.csv", "w");
        #endif
        #ifndef _S_LOCK_TRIE
            fp = fopen("comparison/rw_lock.csv", "w");
        #endif
    #endif
    #ifndef _NO_HOH_LOCK_TRIE
        fp = fopen("comparison/hoh_lock.csv", "w");
    #endif

    // check file open error
	if(fp == NULL){
        fprintf(stderr, "\nFile open error\n");
        exit(EXIT_FAILURE);
    }

    long* times;

    // write to file
    fprintf(fp, "thread_count write_intensive read_intensive read_write\n");

    for(int i = 1; i <= max_thread_count; i++){
        times = run_workloads(i);
        fprintf(fp, "%d %ld %ld %ld\n", i, *times, *(times+1), *(times+2));
    }

    // close file
    fclose(fp);

    // delete trie
    delete_trie(trie);

    // free all malloc-ed memory
    for(int i = 0; i < 3628800; i++){
        free(query_set[i]);
    }    

    free(query_set);

    return 0;
}

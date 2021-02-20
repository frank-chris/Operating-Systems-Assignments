/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/
#include <stdlib.h>   // for malloc(), srand() and rand()
#include <stdio.h>    // for printf()
#include <time.h>     // for time()
#include <math.h>     // for ceil() and floor()
#include "definitions.h"

void swap(int* x, int* y){
    /*
    Function to swap two integers

        Parameters:
            x(int*): address of first integer variable
            y(int*): address of second integer variable
        
        Returns:
            none
    */
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Note: The work must be generated for the returned pointer
struct workload * generate_workload(WorkloadsT type, int pages, int size)
{   /*
    Function to generate workload depending on type, pages and size

        Parameters:
            type(WorkloadsT): type of the workload
            pages(int): the number of pages in workload
            size(int): the total size of the workload
        
        Returns:
            generated workload(struct workload*)
    */
    struct workload* generated_workload = (struct workload*)malloc(sizeof(struct workload));
    generated_workload->type = type;
    generated_workload->pages = pages;
    generated_workload->size = size;

    switch (type){
        case LOOP:
            return generate_loop(generated_workload);
            break;
        case RANDOM:
            return generate_random(generated_workload);
            break;
        case LOCAL:
            return generate_local(generated_workload);
            break;
        default:
            printf("\nInvalid workload type\n");
            break;
    }
}

struct workload * generate_random(struct workload * w)
{   /*
    Function to generate random workload depending on pages and size

        Parameters:
            w(struct workload*): pointer to the workload
        
        Returns:
            w(struct workload*): pointer to the workload after generating
    */
    srand((unsigned)time(NULL));
    int* work = (int*)malloc(sizeof(int)*(w->size));

    for(int i = 0; i < (w->size); i++){
        // assign a random page for each entry in the workload
        work[i] = (rand()%(w->pages)) + 1;
    }
    w->work = work;

    return w;
}

struct workload * generate_loop(struct workload * w)
{   /*
    Function to generate loop workload depending on pages and size

        Parameters:
            w(struct workload*): pointer to the workload
        
        Returns:
            w(struct workload*): pointer to the workload after generating
    */
    int* work = (int*)malloc(sizeof(int)*(w->size));

    for(int i = 0; i < (w->size); i++){
        work[i] = (i%(w->pages)) + 1;
    }
    w->work = work;

    return w;
}

struct workload * generate_local(struct workload * w)
{   /*
    Function to generate local(80-20) workload depending on pages and size

        Parameters:
            w(struct workload*): pointer to the workload
        
        Returns:
            w(struct workload*): pointer to the workload after generating
    */
    srand((unsigned)time(NULL));
    int* work = (int*)malloc(sizeof(int)*(w->size));

    for(int i = 0; i < (int)ceil(0.2*(w->size)); i++){
        // assign a random number between 1 and 0.8*pages to the first 20% of entries
        work[i] = rand()%((int)floor(0.8*(w->pages))) + 1;
    }

    for(int i = (int)ceil(0.2*(w->size)); i < (w->size); i++){
        // assign a random number between 0.8*pages and pages to the last 80% of entries
        work[i] = rand()%((int)ceil(0.2*(w->pages))) + 1 + (int)floor(0.8*(w->pages));
    }

    for(int i = 0; i < (w->size); i++){
        // shuffle the workload randomly
        swap(&work[i], &work[rand()%(w->size)]);
    }
    w->work = work;

    return w;
}
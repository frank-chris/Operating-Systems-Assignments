/*
INSTRUCTIONS:
This file must include only the main function
The main function should print out the various cases
and finally print out tables showing the performance
of various policies with the different workloads.
Vary the cache size.

You may include a function or 2 to ease the printing of tables.

*/

#include "definitions.h"
#include <stdlib.h>  // for exit(), EXIT_FAILURE, atoi()
#include <stdio.h>   // for printf(), fprintf(), fopen(), fclose(), FILE, stderr

void write_results(struct workload* w, char* filename, int max_cache_size){
	/*
    Function to write results into a CSV file

        Parameters:
            w(struct workload*): pointer to the workload
			filename(char*): name to use for CSV file
			max_cache_size(int): maximum cache size that should be considered
        
        Returns:
            none
    */
	FILE* fp;

	// open file
	fp = fopen(filename, "w");

	// file open error handling
	if(fp == NULL){
        fprintf(stderr, "\nFile open error\n");
        exit(EXIT_FAILURE);
    }
	
	// writing into file
	fprintf(fp, "cache_size FIFO LRU LRUapprox RANDOM %d %d\n", w->pages, w->size);
	for (int cache_size = 1; cache_size <= max_cache_size; cache_size++){
		fprintf(fp, "%d %f %f %f %f\n", cache_size, 
				policy_FIFO(w, cache_size),
				policy_LRU(w, cache_size),
				policy_LRUapprox(w, cache_size),
				policy_RANDOM(w, cache_size));
	}
	//closing file
	fclose(fp);
}

int main(int argc, char *argv[])
{	
	if (argc != 3) { // improper usage case
        fprintf(stderr, "\nUsage: %s <pages> <size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	// pages and size arguments
	int pages = atoi(argv[1]);
	int size = atoi(argv[2]);

	printf("\nGenerating random workload: pages = %d, size = %d\n", pages, size);
	struct workload* random_workload = generate_workload(RANDOM, pages, size);

	write_results(random_workload, "data/random.csv", pages);

	printf("\nResults for random workload written into data/random.csv\n");

	printf("\nGenerating looping workload: pages = %d, size = %d\n", pages, size);
	struct workload* loop_workload = generate_workload(LOOP, pages, size);

	write_results(loop_workload, "data/loop.csv", 2*pages);

	printf("\nResults for looping workload written into data/loop.csv\n");

	printf("\nGenerating 80-20 workload: pages = %d, size = %d\n", pages, size);
	struct workload* local_workload = generate_workload(LOCAL, pages, size);

	write_results(local_workload, "data/local.csv", pages);

	printf("\nResults for 80-20 workload written into data/local.csv\n");
	
	return 0;
}
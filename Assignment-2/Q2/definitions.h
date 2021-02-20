/* 
INSTRUCTIONS:

All enum, structs and functions to be declared here.
If a definition is already declared, do not change it.
You may add extra definitions as required.

*/

#ifndef __DEF_H__
#define __DEF_H__

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL ENUMERATORS HERE */

// LOOP - Workload type for Loop
// RANDOM - Workload type for RANDOM
// LOCAL - 80-20 workload type

typedef enum enum_workloads{LOOP, RANDOM, LOCAL}WorkloadsT;

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL STRUCTS HERE */

// struct for queue node to be used in FIFO
struct queue_node { 
    int key; 
    struct queue_node* next; 
}; 

// struct for queue to be used in FIFO
struct queue { 
    struct queue_node* front;
	struct queue_node* rear; 
}; 

// struct for workloads
struct workload
{
	// Defines type of workload
	WorkloadsT type;
	// The number of pages in workload
	int pages;
	// The total size of the workload
	int size;
	// The final work to run the policy
	int * work;
};

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL FUNCTION DEFINITIONS HERE */

// function to swap two integers
void swap(int* x, int* y);

// workload generation functions
struct workload * generate_workload(WorkloadsT type, int pages, int size);
struct workload * generate_random(struct workload * w);
struct workload * generate_loop(struct workload * w);
struct workload * generate_local(struct workload * w);

// helper functions for policies
int search_array(int arr[], int item, int arr_len);
int search_LRU_array(int arr[][2], int item, int arr_len);
int least_recent(int arr[][2], int arr_len);
int least_recent_approx(int arr[][2], int arr_len, int* i);
struct queue_node* init_node(int k);
struct queue* init_queue();
void enqueue(struct queue* q, int k);
void dequeue(struct queue* q);
int search_queue(struct queue* q, int item);

// policy functions
float policy_FIFO(struct workload * w, int cache_size);
float policy_LRU(struct workload * w, int cache_size);
float policy_RANDOM(struct workload * w, int cache_size);
float policy_LRUapprox(struct workload * w, int cache_size);

// function to write results to CSV file
void write_results(struct workload* w, char* filename, int max_cache_size);

#endif /* __DEF_H__ */

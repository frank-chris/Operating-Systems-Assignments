/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include <time.h>     // for time()
#include <stdlib.h>   // for malloc(), free(), srand() and rand()
#include "definitions.h"

int search_array(int arr[], int item, int arr_len){
	/*
	Function to search for 'item' in array 'arr'

		Parameters:
			arr[](int array): the array in which you want to search
			item(int): the element you want to search for
			arr_len(int): the length of the array

		Returns:
			1 if 'item' found in 'arr'
			0 if 'item' not found in 'arr'
	*/
	for(int i = 0; i < arr_len; i++){ 
        if(arr[i] == item){
            return 1;
		}
	}
    return 0; 
}

int search_LRU_array(int arr[][2], int item, int arr_len){
	/*
	Function to search for 'item' in array 'arr'

		Parameters:
			arr[][2](int array-2D): the array in which you want to search
			item(int): the element you want to search for
			arr_len(int): the length of the array

		Returns:
			i(int): index at which 'item' found in 'arr', if it was found
			-1 if 'item' not found in 'arr'
	*/
	for(int i = 0; i < arr_len; i++){
		if(arr[i][0] == item){
			return i;
		}
	}
	return -1;
}

int least_recent(int arr[][2], int arr_len){
	/*
	Function to find least recently used cache entry for LRU

		Parameters:
			arr[][2](int array-2D): the cache array
			arr_len(int): the length of the array

		Returns:
			min_index(int): the index of the least recently used entry
	*/

	// to find the least recently used cache entry,
	// just find the entry with the least value in the second dimension
	// (the second dimension stores the value of a counter when the 
	// entry was last used)
	int minimum = arr[0][1];
	int min_index = 0;
	for(int i = 0; i < arr_len; i++){
		if(arr[i][1] < minimum){
			minimum = arr[i][1];
			min_index = i;
		}
	}
	return min_index;
}

int least_recent_approx(int arr[][2], int arr_len, int* i){
	/*
	Function to find least recently used cache entry for approx LRU

		Parameters:
			arr[][2](int array-2D): the cache array
			arr_len(int): the length of the array
			i(int*): address of the clockhand variable(in clock algorithm)

		Returns:
			*i(int): the index of the least recently used entry
	*/
	// this loop performs the traversal of the clockhand in clock algorithm
	while(1){
		if(arr[*i][1] == 0){
			return *i;
		}
		else{
			arr[*i][1] = 0;
			if(*i == (arr_len - 1)){
				*i = 0;
			}
			else{
				(*i) += 1;
			}
		}
	}
}

struct queue_node* init_node(int k){
	/*
	Function to initialise a queue node

		Parameters:
			k(int): the value with which the node is to be initialised

		Returns:
			temp(struct queue_node*): pointer to the initialised node
	*/
    struct queue_node* temp = (struct queue_node*)malloc(sizeof(struct queue_node));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

struct queue* init_queue(){
	/*
	Function to initialise a queue

		Parameters:
			none

		Returns:
			q(struct queue*): pointer to the initialised queue
	*/
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
    return q;
}

void enqueue(struct queue* q, int k){
	/*
	Function to insert an element into a queue

		Parameters:
			q(struct queue*): the queue into which you want to insert
			k(int): the element you want to insert

		Returns:
			none
	*/
    struct queue_node* temp = init_node(k);

    if(q->rear == NULL){
        q->front = temp;
		q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

void dequeue(struct queue* q){ 
	/*
	Function to remove an element from a queue

		Parameters:
			q(struct queue*): the queue from which you want to remove

		Returns:
			none
	*/
    if (q->front == NULL)
        return;

    struct queue_node* temp = q->front;
    q->front = q->front->next;

    if(q->front == NULL){
        q->rear = NULL;
	}
    free(temp);
}

int search_queue(struct queue* q, int item){
	/*
	Function to search for an element in a queue

		Parameters:
			q(struct queue*): the queue in which you want to search
			item(int): the element you want to search for

		Returns:
			1 if 'item' found in 'q'
			0 if 'item' not found in 'q'
	*/
	struct queue_node* temp = q->front;
	while(temp != NULL){
		if(temp->key == item){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

float policy_FIFO(struct workload * w, int cache_size)
{	/*
	Function for FIFO policy

		Parameters:
			w(struct queue*): pointer to workload
			cache_size(int): size of the cache

		Returns:
			hit_rate(float): the hit-rate achieved by FIFO on given workload
	*/
	float hit_rate = 0;
	int hit_count = 0;
	int cache_entry_count = 0;
	struct queue* cache = init_queue(); 

	for(int i = 0; i < (w->size); i++){
		if(search_queue(cache, w->work[i])){
			// page found in cache
			hit_count += 1;
		}
		else{
			// page not found in cache
			if(cache_entry_count < cache_size){
				// cache is not full
				enqueue(cache, w->work[i]);
				cache_entry_count += 1;
			}
			else{
				//cache is full -> replace
				dequeue(cache);
				enqueue(cache, w->work[i]);
			}
		}
	}
	hit_rate = ((float)hit_count)/((float)(w->size));

	return hit_rate;
}

float policy_LRU(struct workload * w, int cache_size)
{	/*
	Function for LRU policy

		Parameters:
			w(struct queue*): pointer to workload
			cache_size(int): size of the cache

		Returns:
			hit_rate(float): the hit-rate achieved by LRU on given workload
	*/
	float hit_rate = 0;
	int hit_count = 0;
	int cache_entry_count = 0;
	int cache[cache_size][2];
	int search_result;

	for(int i = 0; i < (w->size); i++){
		search_result = search_LRU_array(cache, w->work[i], cache_size);
		if(search_result != -1){
			// page found in cache
			hit_count += 1;
			cache[search_result][1] = i;
		}
		else{
			// page not found in cache
			if(cache_entry_count < cache_size){
				// cache is not full
				cache[cache_entry_count][0] = w->work[i];
				cache[cache_entry_count][1] = i;
				cache_entry_count += 1;
			}
			else{
				//cache is full -> replace
				cache[least_recent(cache, cache_size)][0] = w->work[i];
				cache[least_recent(cache, cache_size)][1] = i;
			}
		}
	}
	hit_rate = ((float)hit_count)/((float)(w->size));

	return hit_rate;
}

float policy_LRUapprox(struct workload * w, int cache_size)
{	/*
	Function for approx LRU policy

		Parameters:
			w(struct queue*): pointer to workload
			cache_size(int): size of the cache

		Returns:
			hit_rate(float): the hit-rate achieved by approx LRU on given workload
	*/
	float hit_rate = 0;
	int hit_count = 0;
	int cache_entry_count = 0;
	int cache[cache_size][2];
	int search_result;
	int clock_hand = 0;

	for(int i = 0; i < (w->size); i++){
		search_result = search_LRU_array(cache, w->work[i], cache_size);
		if(search_result != -1){
			// page found in cache
			hit_count += 1;
			cache[search_result][1] = 1;
		}
		else{
			// page not found in cache
			if(cache_entry_count < cache_size){
				// cache is not full
				cache[cache_entry_count][0] = w->work[i];
				cache[cache_entry_count][1] = 0;
				cache_entry_count += 1;
			}
			else{
				//cache is full -> replace
				cache[least_recent_approx(cache, cache_size, &clock_hand)][0] = w->work[i];
				cache[least_recent_approx(cache, cache_size, &clock_hand)][1] = 1;
			}
		}
	}
	hit_rate = ((float)hit_count)/((float)(w->size));

	return hit_rate;
}

float policy_RANDOM(struct workload * w, int cache_size)
{	/*
	Function for RANDOM policy

		Parameters:
			w(struct queue*): pointer to workload
			cache_size(int): size of the cache

		Returns:
			hit_rate(float): the hit-rate achieved by RANDOM on given workload
	*/
	float hit_rate = 0;
	int cache[cache_size];
	int hit_count = 0;
	int cache_entry_count = 0;
	srand((unsigned)time(NULL));

	for(int i = 0; i < (w->size); i++){
		if(search_array(cache, w->work[i], cache_size)){
			// page found in cache
			hit_count += 1;
		}
		else{
			// page not found in cache
			if(cache_entry_count < cache_size){
				// cache is not full
				cache[cache_entry_count] = w->work[i];
				cache_entry_count += 1;
			}
			else{
				//cache is full -> replace
				cache[rand()%cache_size] = w->work[i];
			}
		}
	}
	hit_rate = ((float)hit_count)/((float)(w->size));

	return hit_rate;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strlen()
#include "trie.h"

bool is_leaf(trie_node_t node){
    /*
        Function to check if a node is a leaf

            Parameters:
                node(trie_node_t): the node you want to check

            Returns:
                true if node is a leaf
                false if node is not a leaf
    */
    for(int i = 0; i < 26; i++){
        if(node->children[i]){
            return false;
        }
    }
    return true;
}

trie_node_t init_trie_node(void){
    /*
        Function to initialise a trie node

            Parameters:
                none

            Returns:
                new_node(trie_node_t): the created and initialised node
    */
    trie_node_t new_node = NULL;
    new_node = (trie_node_t)malloc(sizeof(_trie_node_t));

    if(new_node){
        new_node->is_end = false;
        new_node->value = 0;
        for(int i = 0; i < 26; i++){
            new_node->children[i] = NULL;
        }
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_init(&(new_node->node_lock), NULL);
        #endif
    }
    return new_node;
}

trie_t init_trie(void){
    /*
        Function to initialise a trie

            Parameters:
                none

            Returns:
                new_trie(trie_t): the created and initialised trie
    */
    trie_t new_trie = NULL;
    new_trie = (trie_t)malloc(sizeof(_trie_t));

    if(new_trie){
        new_trie->head = init_trie_node();
        #ifdef _NO_HOH_LOCK_TRIE
            #ifdef _S_LOCK_TRIE
                pthread_mutex_init(&(new_trie->s_lock), NULL);
            #endif
            #ifndef _S_LOCK_TRIE
                pthread_rwlock_init(&(new_trie->rw_lock), NULL);
            #endif
        #endif
    }
    return new_trie;
}

void insert(trie_t trie, char* key, int value){
    /*
        Function to insert a key-value pair into the trie

            Parameters:
                trie(trie_t): the trie into which you want to insert
                key(char*): the word you want to insert
                value(int): the value corresponding to the word

            Returns:
                none
    */
    int length = strlen(key);
    int i; 

    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_wrlock(&(trie->rw_lock));
        #endif
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(trie->head->node_lock));
    #endif
    
    trie_node_t temp = trie->head;

    for(int depth = 0; depth < length; depth++){
        i = CHAR_TO_INDEX(key[depth]);
        if(!temp->children[i]){
            temp->children[i] = init_trie_node();
        }
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_t* temp_lock = &(temp->node_lock);
        #endif
        temp = temp->children[i];
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&(temp->node_lock));
            pthread_mutex_unlock(temp_lock);
        #endif
    }
    temp->value = value;
    temp->is_end = true;

    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&(temp->node_lock));
    #endif

    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->rw_lock));
        #endif
    #endif
}

int find(trie_t trie, char* key, int* val_ptr){
    /*
        Function to find a key in a trie

            Parameters:
                trie(trie_t): the trie in which you want to find
                key(char*): the word you want to find
                val_ptr(int*): pointer pointing to the variable that will
                                store the value of the key found

            Returns:
                -1 if key not found in trie
                0 if key found in trie
    */
    int length = strlen(key);
    int i;
    bool ret = false;
    int ret_value = -1;

    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_rdlock(&(trie->rw_lock));
        #endif
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(trie->head->node_lock));
    #endif

    trie_node_t temp = trie->head;

    for(int depth = 0; depth < length; depth++){
        i = CHAR_TO_INDEX(key[depth]);
        if(!temp->children[i]){
            ret = true;
            break;
        }
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_t* temp_lock = &(temp->node_lock);
        #endif
        temp = temp->children[i];
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&(temp->node_lock));
            pthread_mutex_unlock(temp_lock);
        #endif
    }
    if(temp->is_end && temp != NULL && ret == false){
        *val_ptr = temp->value;
        ret_value = 0;
    }
    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&(temp->node_lock));
    #endif
    
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->rw_lock));
        #endif
    #endif
    return ret_value;
}

void delete_kv(trie_t trie, char* key){
    /*
        Function to delete a key in a trie

            Parameters:
                trie(trie_t): the trie in which you want to delete
                key(char*): the word/key you want to delete

            Returns:
                none
    */
    int length = strlen(key);
    int i;
    bool ret = false;

    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_wrlock(&(trie->rw_lock));
        #endif
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(trie->head->node_lock));
    #endif

    trie_node_t temp = trie->head;

    for(int depth = 0; depth < length; depth++){
        i = CHAR_TO_INDEX(key[depth]);
        if(!temp->children[i]){
            ret = true;
            break;
        }
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_t* temp_lock = &(temp->node_lock);
        #endif
        temp = temp->children[i];
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&(temp->node_lock));
            pthread_mutex_unlock(temp_lock);
        #endif
    }
    if(temp->is_end && temp != NULL && ret == false){
        temp->is_end = false;
    }
    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&(temp->node_lock));
    #endif
    
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->rw_lock));
        #endif
    #endif
}

void recursive_keys_with_prefix(trie_node_t node, char** list, int* key_count, char* prefix){
    /*
        Recursive helper function for keys_with_prefix()

            Parameters:
                node(trie_node_t): the current node
                list(char**): list of keys with the given prefix
                key_count(int*): count variable to keep track of keys found so far
                prefix(char*): the prefix

            Returns:
                none
    */
    if(node->is_end == true){
        char* key = malloc(sizeof(char)*(strlen(prefix)+1));
        strcpy(key, prefix);
        list[*key_count] = key;
        list[*key_count+1] = NULL;
        *key_count += 1;
    }
    for(int i = 0; i < 26; i++){
        if(node->children[i]){
            char* modified_prefix = malloc(sizeof(char)*(strlen(prefix)+2));
            strcpy(modified_prefix, prefix);
            modified_prefix[strlen(prefix)] = INDEX_TO_CHAR(i);
            modified_prefix[strlen(prefix)+1] = '\0';
            #ifndef _NO_HOH_LOCK_TRIE
                pthread_mutex_lock(&(node->children[i]->node_lock));
                pthread_mutex_unlock(&(node->node_lock));
            #endif
            recursive_keys_with_prefix(node->children[i], list, key_count, modified_prefix);
            #ifndef _NO_HOH_LOCK_TRIE
                pthread_mutex_lock(&(node->node_lock));
            #endif
            free(modified_prefix);
        }
    }
    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&(node->node_lock));
    #endif
}

char** keys_with_prefix(trie_t trie, char* prefix){
    /*
        Function to find keys having the given prefix

            Parameters:
                trie(trie_t): the trie in which you want to find
                prefix(char*): the prefix

            Returns:
                list(char**): list of keys that have the given prefix
    */
    char** list = malloc(sizeof(char*)*5000);
    list[0] = NULL;
    int length = strlen(prefix);
    int i;
    bool ret = false;

    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_rdlock(&(trie->rw_lock));
        #endif
    #endif

    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(trie->head->node_lock));
    #endif
    
    trie_node_t temp = trie->head;
    
    for(int depth = 0; depth < length; depth++){
        i = CHAR_TO_INDEX(prefix[depth]);
        if(!temp->children[i]){
            ret = true;
            break;
        }
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_t* temp_lock = &(temp->node_lock);
        #endif
        temp = temp->children[i];
        #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&(temp->node_lock));
            pthread_mutex_unlock(temp_lock);
        #endif
    }
    if(ret == false){
        int key_count = 0;
        recursive_keys_with_prefix(temp, list, &key_count, prefix);
    }
    
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->rw_lock));
        #endif
    #endif
    return list;
}

void recursive_delete_trie(trie_t trie, trie_node_t node){
    /*
        Recursive helper function for delete_trie()

            Parameters:
                trie(trie_t): the trie
                node(trie_node_t): the current node

            Returns:
                none
    */
    if(node == NULL){
        return;
    }
    for(int i = 0; i < 26; i++){
        if(node->children[i]){
            #ifndef _NO_HOH_LOCK_TRIE
                pthread_mutex_lock(&(node->children[i]->node_lock));
                pthread_mutex_unlock(&(node->node_lock));
            #endif
            recursive_delete_trie(trie, node->children[i]);
            #ifndef _NO_HOH_LOCK_TRIE
                pthread_mutex_lock(&(node->node_lock));
            #endif
        }
    }
    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&(node->node_lock));
    #endif
    free(node);
}

void delete_trie(trie_t trie){
    /*
        Function to delete a trie

            Parameters:
                trie(trie_t): the trie to be deleted

            Returns:
                none
    */
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_lock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_wrlock(&(trie->rw_lock));
        #endif
    #endif
    #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&(trie->head->node_lock));
    #endif

    recursive_delete_trie(trie, trie->head);
    
    #ifdef _NO_HOH_LOCK_TRIE
        #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&(trie->s_lock));
        #endif
        #ifndef _S_LOCK_TRIE
            pthread_rwlock_unlock(&(trie->rw_lock));
        #endif
    #endif
    free(trie);
}

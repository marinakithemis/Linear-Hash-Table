#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/structs.h"
#include "../headers/header.h"

////initialization functions for each struct of the hash table////

struct Hash_Table* initialize_hashtable(int init_size, int bucket_entries)    
{
    struct Hash_Table* hash_table = malloc(sizeof(struct Hash_Table));
    struct Bucket **buckets = calloc(init_size, sizeof(struct Bucket*));

    if(hash_table == NULL || buckets == NULL){   //if malloc fails
        printf("Problem in memory allocation\n");
        exit(0);
    }

    for(int i = 0; i < init_size; i++){
        buckets[i] = initialize_bucket(bucket_entries);
    }

    hash_table->buckets = buckets;
    hash_table->num_of_keys = 0;
    hash_table->round = 0;
    hash_table->p = 0;
    hash_table->hash_init_size = init_size;
    hash_table->hash_cur_size = init_size;
    hash_table->bucket_init_size = bucket_entries;
    hash_table->hash_temp_size = init_size;
    return hash_table;
}

struct Hash_Table* reinitialize_hashtable(struct Hash_Table *hash_table)   //function used in split to add another bucket to the hash table
{
    int cur_size = hash_table->hash_cur_size;
    struct Bucket** new_buckets = realloc(hash_table->buckets, (cur_size + 1)*sizeof(struct Bucket*));
    if(new_buckets != NULL){
        hash_table->buckets = new_buckets;
    }
    else{                  //if realloc fails
        printf("Problem in memory allocation\n");
        exit(0);
    }

    struct Bucket* bucket = initialize_bucket(hash_table->bucket_init_size);
    hash_table->buckets[cur_size] = bucket;
    hash_table->hash_cur_size += 1;
    return hash_table;
}


struct Voter* initialize_voter(int PIN, char* fname, char* sname, int PostalCode)
{
    struct Voter* voter = malloc(sizeof(struct Voter));
    voter->fname = malloc(MAX_SIZE*sizeof(char));
    voter->sname = malloc(MAX_SIZE*sizeof(char));

    if(voter == NULL || voter->fname == NULL || voter->sname == NULL){
        printf("Problem in memory allocation\n");
        exit(0);
    }

    voter->PIN = PIN;
    voter->fname = strcpy(voter->fname,fname);
    voter->sname = strcpy(voter->sname,sname);
    voter->postal_code = PostalCode;
    voter->has_voted = 'N';
    return voter;

}

struct Bucket* initialize_bucket(int bucket_entries)
{
    struct Bucket* bucket = malloc(sizeof(struct Bucket));
    struct Voter **voters = calloc(bucket_entries,sizeof(struct Voter*));

    if(bucket == NULL || voters == NULL){
        printf("Problem in memory allocation\n");
        exit(0);
    }

    bucket->voters = voters;
    bucket->bucket_entries = bucket_entries;
    bucket->bucket_cur_size = bucket_entries;
    bucket->overflow_bucket = NULL;
    return bucket;
}

//// hash function where i is the round, k is the key and m is the initial number of buckets/////

int h(int i, int k, int m)
{
    int power = pow(2, i);
    int h_i = k % (power * m);
    return h_i;
}

//// hash insert function ////

void hash_insert(struct Hash_Table *Hash_Table, struct Voter *voter)
{
    int init_size = Hash_Table->hash_init_size, round = Hash_Table->round, p = Hash_Table->p;
    int key = voter->PIN;
    int bucket_size = Hash_Table->bucket_init_size;
    int h_i = h(round, key, init_size);
    int h_i_next = h(round+1, key, init_size);

    if (h_i >= p){
        struct Bucket* temp_bucket = Hash_Table->buckets[h_i];

        while(temp_bucket->overflow_bucket != NULL){
            temp_bucket = temp_bucket->overflow_bucket;
        }

        int bucket_cur_size = temp_bucket->bucket_cur_size;

        if (bucket_cur_size > 0){        //if the bucket has empty space for another voter
            temp_bucket->voters[bucket_size - bucket_cur_size] = voter;
            temp_bucket->bucket_cur_size -= 1;
            printf("Inserted %d %s %s %d %c\n",voter->PIN, voter->sname, voter->fname, voter->postal_code, voter->has_voted);
        }

        else{                           //else we have to make an overflow bucket and add it to the list of overflow buckets
            struct Bucket* overflow_bucket =  initialize_bucket(bucket_size);
            overflow_bucket->voters[bucket_size - overflow_bucket->bucket_cur_size] = voter;
            overflow_bucket->bucket_cur_size -= 1;
            temp_bucket->overflow_bucket = overflow_bucket;
            printf("Inserted %d %s %s %d %c\n",voter->PIN, voter->sname, voter->fname, voter->postal_code, voter->has_voted);
        }
    }

    else{     //we do the exact same thing as before but instead of h_i we use the hash function of the next round h_i_next (were round = round + 1)

        struct Bucket* temp_bucket = Hash_Table->buckets[h_i_next];
        while(temp_bucket->overflow_bucket != NULL){
            temp_bucket = temp_bucket->overflow_bucket;
        }

        int bucket_cur_size = temp_bucket->bucket_cur_size;
        if (bucket_cur_size > 0){
            temp_bucket->voters[bucket_size - bucket_cur_size] = voter;
            temp_bucket->bucket_cur_size -= 1;
            printf("Inserted %d %s %s %d %c\n",voter->PIN, voter->sname, voter->fname, voter->postal_code, voter->has_voted);
        }

        else{
            struct Bucket* overflow_bucket = initialize_bucket(bucket_size);
            overflow_bucket->voters[bucket_size - overflow_bucket->bucket_cur_size] = voter;
            overflow_bucket->bucket_cur_size -= 1;
            temp_bucket->overflow_bucket = overflow_bucket;
            printf("Inserted %d %s %s %d %c\n",voter->PIN, voter->sname, voter->fname, voter->postal_code, voter->has_voted);
        }
    }

    Hash_Table->num_of_keys += 1;
    hash_split(Hash_Table);
    
}

void hash_split(struct Hash_Table *Hash_Table)     //checks if we need to split the bucket that p shows and if yes it splits it
{
    int bucket_size = Hash_Table->bucket_init_size;
    int p = Hash_Table->p, cur_size = Hash_Table->hash_cur_size, round = Hash_Table->round;
    int num_of_keys = Hash_Table->num_of_keys;                 //total number of voters that the hash table has up until now
    int key_capacity = cur_size * bucket_size;                 // how many keys can we store in the hash table 
    float l = (float)num_of_keys/(float)key_capacity;         //l is the load that we need to check every time we insert a new voter into the hash table

    if(l > 0.75){
        struct Bucket* temp_bucket = Hash_Table->buckets[p];
        Hash_Table = reinitialize_hashtable(Hash_Table);     //we add a bucket to our hash table
        struct Bucket* old_bucket;
        Hash_Table->buckets[p] = initialize_bucket(bucket_size);  //we initialize the bucket again so that it is empty

        while(temp_bucket != NULL){
            int i = 0;
            while(i < bucket_size && temp_bucket->voters[i] != NULL){  //we rehash each voter of temp_bucket now using the hash function of the next round h_i_next

                int key = temp_bucket->voters[i]->PIN;
                int h_i_next = h((Hash_Table->round)+1, key, Hash_Table->hash_init_size);
                struct Bucket* temp_bucket2 = Hash_Table->buckets[h_i_next];

                ////same steps with hash_insert////

                while(temp_bucket2->overflow_bucket != NULL){
                    temp_bucket2 = temp_bucket2->overflow_bucket;
                }   

                int bucket_cur_size = temp_bucket2->bucket_cur_size;

                if (bucket_cur_size > 0){
                    temp_bucket2->voters[bucket_size - bucket_cur_size] = temp_bucket->voters[i];
                    temp_bucket2->bucket_cur_size -= 1;
                }

                else{
                    struct Bucket* overflow_bucket = initialize_bucket(bucket_size);
                    overflow_bucket->voters[bucket_size - overflow_bucket->bucket_cur_size] = temp_bucket->voters[i];
                    overflow_bucket->bucket_cur_size -= 1;
                    temp_bucket2->overflow_bucket = overflow_bucket;
                }
                i++;      
            }

            old_bucket = temp_bucket;
            temp_bucket = temp_bucket->overflow_bucket;
            free(old_bucket->voters);
            free(old_bucket);
        }
        Hash_Table->p += 1;
        int y = 2 * (Hash_Table->hash_temp_size);

        //we need to check if the hash table has doubled its size, if yes, all buckets have been splitted and we have to move to the next round and reinitialize p
        if(Hash_Table->hash_cur_size == y){
            Hash_Table->hash_temp_size = y;
            Hash_Table->p = 0;
            Hash_Table->round += 1;
        }
    }

    else{
        return;
    }
    
}

////this function prints the hash table, i made it to help me debug my program////

void print_hash(struct Hash_Table *Hash_Table)
{
    int size = Hash_Table->hash_cur_size;
    for(int i = 0; i < size; i++){
        printf("%d :",i );
        struct Bucket* temp_bucket = Hash_Table->buckets[i];
        while(temp_bucket != NULL){
            for(int j = 0; j < Hash_Table->bucket_init_size; j++){
                if(temp_bucket->voters[j] != NULL){
                    printf("%d ", temp_bucket->voters[j]->PIN);
                }
            }
            temp_bucket = temp_bucket->overflow_bucket;
        }
        printf("\n");
    }
}

//// this function searches for a specific voter based on their PIN and if it finds them it returns the voter, if not it returns NULL /////

struct Voter* hash_search(struct Hash_Table *hash_table, int PIN)
{
    int init_size = hash_table->hash_init_size, round = hash_table->round, p = hash_table->p, bucket_size = hash_table->bucket_init_size;
    int key = PIN;
    int h_i = h(round, key, init_size);
    int h_i_next = h(round+1, key, init_size);

    if (h_i >= p){
        struct Bucket* temp_bucket = hash_table->buckets[h_i];
        while(temp_bucket != NULL){
            for(int j = 0; j < bucket_size; j++){
                struct Voter* voter = temp_bucket->voters[j];
                if(voter != NULL && voter->PIN == key){
                    return voter;
                }
            }
            temp_bucket = temp_bucket->overflow_bucket;
        }
    }

    else{
        struct Bucket* temp_bucket = hash_table->buckets[h_i_next];
        while(temp_bucket != NULL){
            for(int j = 0; j < bucket_size; j++){
                struct Voter* voter = temp_bucket->voters[j];
                if(voter != NULL && voter->PIN == key){
                    return voter;
                }
            }
            temp_bucket = temp_bucket->overflow_bucket;
        }
    }

    return NULL;
}

//// when user types the l command this function is called ////

void search_command(struct Hash_Table *hash_table, int PIN)
{
    struct Voter* voter = hash_search(hash_table, PIN);
    if(voter != NULL){
        printf("%d %s %s %d %c\n", voter->PIN, voter->fname, voter->sname, voter->postal_code, voter->has_voted); 
    }
    else{
        printf("Participant %d not in cohort\n", PIN);
    }
}

//// when user types the m command this function is called ////

void mark_has_voted_command(struct Hash_Table *hash_table, struct List** list, int PIN)
{
    struct Voter* voter = hash_search(hash_table, PIN);
    if(voter != NULL){
        if(voter->has_voted == 'N'){
            voter->has_voted = 'Y';
            printf("%d Marked Voted\n", PIN);

             //we have to insert the voter to the list too (because he has voted)//
            struct Voter_Node* voter_node = initialize_voter_node(voter); 
            list_insert(list, voter_node);
        }

        else{
            printf("%d already marked voted\n", PIN);
        }
    }

    else{
        printf("%d does not exist\n",PIN);
    }
}

//// when user types the perc command this function is called ////

float voter_perc(struct Hash_Table* hash_table, struct List* list)
{
    int total_voters = hash_table->num_of_keys;
    int has_voted = num_of_voters(list);
    float perc = ((float)has_voted / (float)total_voters)*100;
    return perc;
}

//// function to destroy the hash table and return the bytes that it freed ////

int free_hashtable(struct Hash_Table* hash_table)
{
    int size = hash_table->hash_cur_size;
    int bytes_freed = 0;
    for(int i = 0; i < size; i++){
        struct Bucket* temp_bucket = hash_table->buckets[i];
        struct Bucket* old_bucket;
        while(temp_bucket != NULL){
            for(int j = 0; j < hash_table->bucket_init_size; j++){
                if(temp_bucket->voters[j] != NULL){
                    free(temp_bucket->voters[j]->fname);
                    free(temp_bucket->voters[j]->sname);
                    free(temp_bucket->voters[j]);
                    bytes_freed += 2*MAX_SIZE*sizeof(char) + sizeof(struct Voter*);
                }
            }
            bytes_freed += sizeof(struct Bucket) + sizeof(struct Voter*);
            old_bucket = temp_bucket;
            temp_bucket = temp_bucket->overflow_bucket;
            free(old_bucket->voters);
            free(old_bucket);
        }
        bytes_freed += sizeof(struct Bucket);
        free(temp_bucket);
    }
    bytes_freed += sizeof(struct Hash_Table) + size*sizeof(struct Bucket*);
    free(hash_table->buckets);
    free(hash_table);
    return bytes_freed;
}

//// this function helps me check if the string I pass to it is an integer or not, I use it to check the input ////
bool is_integer(char* value) 
{
    int i = 0;
    while(value[i] != '\0'){
        if(isdigit(value[i]) == 0){
            return false;
        }
        i++;
    }
    return true;
}

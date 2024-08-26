#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/structs.h"
#include "../headers/header.h"


////initialization functions for each struct of the P.C List////

struct List* initialize_list()
{
    struct List* list = malloc(sizeof(struct List));

    if(list == NULL){
        printf("Problem in memory allocation\n");
        exit(0);
    }

    struct List_Node* head = initialize_list_node(-1, NULL);
    list->head = head;
    list->tail = head;
    list->head->next = NULL;
    list->total_voters = 0;
    return list;
}

struct List_Node* initialize_list_node(int Postal_Code, struct Voter_Node* voter)
{
    struct List_Node* node = malloc(sizeof(struct List_Node));

    if(node == NULL){
        printf("Problem in memory allocation\n");
        exit(0);
    }

    node->next = NULL;
    node->num_voters = 1;
    node->Postal_Code = Postal_Code; 
    node->voter = voter;
    node->prev = NULL;
    return node;

}

struct Voter_Node* initialize_voter_node(struct Voter* voter)
{
    struct Voter_Node* voter_node = malloc(sizeof(struct Voter_Node));

    if(voter_node == NULL){
        printf("Problem in memory allocation\n");
        exit(0);
    }

    voter_node->voter = voter;
    voter_node->next = NULL;
    return voter_node;
}

//// function to insert a new voter to my list ////

void list_insert(struct List **list, struct Voter_Node* voter_node)
{
    int key  = voter_node->voter->postal_code;
    struct List_Node *temp_node = (*list)->head;
    struct List_Node *prev_node = (*list)->head;

    //if the postal code already exists on the list//
    while(temp_node != NULL && temp_node->Postal_Code != -1){             
        if(temp_node->Postal_Code == key){
            struct Voter_Node* temp_voter = temp_node->voter;
            while(temp_voter->next != NULL){
                temp_voter = temp_voter->next;
            }
            temp_voter->next = voter_node;
            temp_node->num_voters += 1;
            (*list)->total_voters += 1;
            struct List_Node *cur_node = temp_node;
            while(temp_node->prev != NULL && cur_node->num_voters > temp_node->prev->num_voters){
                temp_node = temp_node->prev;
            }
            struct List_Node *temp = temp_node;
            node_swap(list, temp_node, cur_node);   //we have to keep the nodes of the list in descending order (based on the number of voters each node has)
            return;
        }
        prev_node = temp_node;
        temp_node = temp_node->next;
    }

    //if list is empty
    if(prev_node->Postal_Code == -1){ 
        prev_node->voter = voter_node;
        prev_node->Postal_Code = key;
        prev_node->next = NULL;
        prev_node->prev = NULL;
        prev_node->num_voters = 1;
        (*list)->total_voters += 1;
        (*list)->tail = prev_node;
    }

    //if the postal code doesn't exist in the list, we insert it at the end of the list
    else{
        struct List_Node* new_node = initialize_list_node(key, voter_node);
        struct List_Node *prev_node = (*list)->tail;
        prev_node->next = new_node;
        new_node->prev = prev_node;
        (*list)->total_voters += 1;
        (*list)->tail = new_node;
    }
}

//removes node from the list and inserts it in the correct place
void node_swap(struct List** list, struct List_Node* node1, struct List_Node* node2)
{
    //checking the arguments
    if(node1 == node2 || (*list)->head == NULL || (*list)->head->next == NULL){  
        return;
    }

    if(node2 == (*list)->tail){
            (*list)->tail = node2->prev;
    }

    //we take node2 out of the list
    if(node2->next != NULL){
        node2->next->prev = node2->prev;
    }

    if(node2->prev != NULL){
        node2->prev->next = node2->next;
    }

    //check if we need to change the lists head
    if(node1 == (*list)->head){
        node2->next = (*list)->head;
        node2->next->prev = node2;
        node2->prev = NULL;
        (*list)->head = node2;
    }

    //we insert node2 in the correct place
    else{
        struct List_Node* temp_node = node1->prev;

        node2->next = temp_node->next;
        if(temp_node->next != NULL){
            node2->next->prev = node2;
        }
        temp_node->next = node2;
        node2->prev = temp_node;
    }
}

//// when user types the v command this function is called ////

int num_of_voters(struct List* list)   //return number of voters who have voted so far, so the number of voters that the list has
{
    int num_of_voters = list->total_voters;
    return num_of_voters;
}

void print_list(struct List* list)   //function to print the contents of the list//
{
    struct List_Node* temp_node = list->head;
    
    if(temp_node->Postal_Code == -1){
        printf("List is empty\n");
        return;
    }

    while(temp_node != NULL){
        printf("%d: %d\n", temp_node->Postal_Code, temp_node->num_voters);
        temp_node = temp_node->next;
    }
}

//// when user types the z command this function is called ////

void zipcode_print(struct List* list, int zipcode)   
{
    struct List_Node* temp_node = list->head;

    while(temp_node != NULL){
        if(temp_node->Postal_Code == zipcode){
            printf("%d Voted in zipcode %d\n", temp_node->num_voters, zipcode);
            struct Voter_Node* voter = temp_node->voter;
            while(voter != NULL){
                printf("%d\n", voter->voter->PIN);
                voter = voter->next;
            }
            return;
        }
        temp_node = temp_node->next;
    }
    
    //if we can't find a node with this zipcode, it means that no one has voted in this zipcode//
    printf("0 voted in zipcode %d\n", zipcode);
}

//// function to destroy the list and return the number of bytes  we freed ////
int free_list(struct List* list)
{
    struct List_Node* temp_node = list->head;
    struct List_Node* old_node = list->head;
    int bytes_freed = 0;

    while(temp_node != NULL){

        struct Voter_Node* voter = temp_node->voter;
        struct Voter_Node *old_voter = voter;
        while(voter != NULL){
            old_voter = voter;
            voter = voter->next;
            bytes_freed += sizeof(struct Voter_Node);
            free(old_voter);
        }
        old_node = temp_node;
        temp_node = temp_node->next;
        bytes_freed += sizeof(struct List_Node);
        free(old_node);
    }
    bytes_freed += sizeof(struct List);
    free(list);
    return bytes_freed;
}

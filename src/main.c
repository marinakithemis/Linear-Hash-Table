#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/header.h"

int main(int argc, char* argv[])
{    

    int init_size ;
    char *filename;
    int bucket_entries;
    
    if (argc != 7){
        printf("Wrong number of arguments\n");
        return -1;
    }

    ////check the flags in the arguments////

    for(int i = 0; i < argc ; i++){
        if(strcmp(argv[i], "-b") == 0){
            if(i+1 < argc){
                if(is_integer(argv[i+1])){       //we have to check if after flag -b we have an integer 
                    bucket_entries = atoi(argv[i+1]);
                }
                else{
                    printf("Wrong arguments\n");
                    return -1;
                }
            }
        }
        else if(strcmp(argv[i], "-f") == 0){
            if(i + 1 < argc){
                filename = argv[i+1];
            }
        }
        else if(strcmp(argv[i], "-s") == 0){   //I added flag -s in the arguments to let the user decide the initial size of the hash table
            if(i + 1 < argc){
                if(is_integer(argv[i+1])){    //same thing as in flag -b
                    init_size = atoi(argv[i+1]);
                }
                else{
                    printf("Wrong arguments\n");
                    return -1;
                }
            }
        }
    }

    ////get the contents of the file////
    FILE *file = fopen(filename, "r");
    char *line = malloc(MAX_SIZE*sizeof(char));
    char *lin = malloc(MAX_SIZE*sizeof(char));

    if (line == NULL) {
        printf("Memory not allocated \n");
        exit(0);
    }

    if ((file == NULL)){
        printf("Error during file opening\n");
        exit(0);
    }

    hash_table Hash_Table = initialize_hashtable(init_size, bucket_entries);
    while (fgets(line, 100, file) != NULL) {

        const char s[2] = " ";
        char* PIN_str = strtok(line, s);
        char* sname = strtok(NULL, s);  //first name
        char* fname = strtok(NULL, s);  //second/last name (lname)
        char* post_code_str = strtok(NULL,s);

        if(PIN_str != NULL && fname != NULL && sname != NULL && post_code_str != NULL){

            PIN_str[strcspn(PIN_str, "\r\n")] = 0;              //I use the strcspn function to remove new lines etc. from strings because they mess up functions like strcmp, is_integer
            post_code_str[strcspn(post_code_str, "\r\n")] = 0;

            if(is_integer(PIN_str) == true && is_integer(post_code_str) == true){
                int PIN = atoi(PIN_str);
                int post_code = atoi(post_code_str);
                if(hash_search(Hash_Table, PIN) == NULL){
                    voter Voter = initialize_voter(PIN, fname, sname, post_code);
                    hash_insert(Hash_Table, Voter);
                }
                else{
                    printf("%d already exists\n",PIN);
                }
                
            }
            else{
                printf("malformed input\n");
                break;
            }
            
        }
        else{
            printf("Malformed input\n");
            break;
        }
       
    }

    fclose(file);

    list List = initialize_list();
    char *command = malloc(MAX_SIZE*sizeof(char));
    while(1){

        printf("Please enter your command:\n");
        fgets(command, sizeof(char) * MAX_SIZE, stdin);
        char *data = strtok(command, " ");       //data string stores the command (eg. l, i, bv, etc.)
        data[strcspn(data, "\r\n")] = 0;         

        if(strcmp(data, "l") == 0){              //l command to search for a specific voter

            char* PIN_str = strtok(NULL, " ");

            if(PIN_str != NULL){

                PIN_str[strcspn(PIN_str, "\r\n")] = 0;

                if(is_integer(PIN_str) == true){
                    int PIN = atoi(PIN_str);
                    search_command(Hash_Table, PIN);
                }
                else{
                   printf("Malformed pin\n");   //pin is not an integer
                }
            }
            else{
                printf("Malformed pin\n");     //there is no pin in the input
            }
        }

        else if(strcmp(data, "i") == 0){   //i command to insert a voter <PIN> <sname> <fname> <postal code>

            char* PIN_str = strtok(NULL, " ");
            char* sname = strtok(NULL," ");
            char* fname = strtok(NULL," ");
            char* post_code_str = strtok(NULL," ");
            
            if(PIN_str != NULL && post_code_str != NULL && fname != NULL && sname != NULL){
                
                PIN_str[strcspn(PIN_str, "\r\n")] = 0;
                post_code_str[strcspn(post_code_str, "\r\n")] = 0;

                if(is_integer(PIN_str) == 1 && is_integer(post_code_str) == 1 && is_integer(fname) == 0 && is_integer(sname) == 0){
                    int PIN = atoi(PIN_str);
                    int post_code = atoi(post_code_str);
                    if(hash_search(Hash_Table, PIN) == NULL){          //if voter doesn't already exist in the hash table, we insert them
                        voter Voter = initialize_voter(PIN, fname, sname, post_code);
                        hash_insert(Hash_Table, Voter);
                    }
                    else{
                        printf("%d already exists\n",PIN);
                    }
                }   
                else{
                    printf("Malformed Input\n");
                }
            }
            else{
                printf("Malformed Input\n");
            }
           
        }

        else if(strcmp(data, "m") == 0){    //command m to set a voters has_voted statue to 'Y'

            char* PIN_str = strtok(NULL," ");

            if(PIN_str != NULL){
                PIN_str[strcspn(PIN_str, "\r\n")] = 0;

                if(is_integer(PIN_str) == true){
                    int PIN = atoi(PIN_str);
                    mark_has_voted_command(Hash_Table, &List, PIN);
                }
                else{
                    printf("Malformed pin\n");
                }
            }
            else{
                printf("Malformed pin\n");
            }
        }

        else if(strcmp(data, "bv") == 0){   //bv command to set the status has_voted to 'Y' of the voters included in the given file

            char* fileofkeys = strtok(NULL, " ");

            if(fileofkeys == NULL){
                printf("Malformed Input\n");
                continue;
            }

            fileofkeys[strcspn(fileofkeys, "\r\n")] = 0;
            FILE *file = fopen(fileofkeys, "r");

            if (lin == NULL) {
                printf("Memory not allocated \n");
                exit(0);
            }

            if ((file == NULL)){
                printf("%s could not be opened\n", fileofkeys);
                continue;
            }

            while (fgets(lin, 100, file) != NULL) {
                char* PIN_str = strtok(lin," ");
                if(PIN_str != NULL){
                    PIN_str[strcspn(PIN_str, "\r\n")] = 0;
                    if(is_integer(PIN_str)){
                        int PIN = atoi(PIN_str);
                        mark_has_voted_command(Hash_Table,&List, PIN);
                    }
                    else{
                        printf("Malformed pin\n");
                        break;
                    }
                }
                else{
                    printf("Malformed pin\n");
                    break;
                }
            }
            fclose(file);
        }

        else if(strcmp(data, "v") == 0){   //v command to print the number of voters who have already voted
            int v = num_of_voters(List);
            printf("Voted so far %d\n",v);
        }

        else if(strcmp(data, "perc") == 0){   //perc command to print the percentage of voters who have already voted compared to the total num of voters
            float perc = voter_perc(Hash_Table, List);
            printf("%f %%\n",perc);
        }

        else if(strcmp(data, "z") == 0){     //z command to print number of voters on a specific postal code and their pins

            char *zipcode_str = strtok(NULL, " ");
            if(zipcode_str != NULL){
                zipcode_str[strcspn(zipcode_str, "\r\n")] = 0;
                if(is_integer(zipcode_str) == true){
                    int zip_code = atoi(zipcode_str);
                    zipcode_print(List, zip_code);
                }
                else{
                    printf("Malformed input\n");
                }
            }
            else{
                printf("Malformed Input\n");
            }
        }

        else if(strcmp(data, "o")== 0){    //o command to print postal codes and the voters who have already voted of each postal code
            print_list(List);
        }

        else if(strcmp(data, "help") == 0){
            printf("1.Press l <PIN> to search for a voter\n");
            printf("2.Press i <PIN> <fname> <lname> <zipcode> to insert a voter\n");
            printf("3.Press m <PIN> to set the has_voted status of the voter to YES\n");
            printf("4.Press bv <filename> to set the has_voted status of the voter inside the file to YES\n");
            printf("5.Press v to see how many voters have voted so far\n");
            printf("6.Press perc to see the percentage of eclectors who have already voted compared to the total number of voters\n");
            printf("7.Press z <zipcode> to see how many voters have voted so far from the <zipcode> and print their PINS\n");
            printf("8.Press o to print the list of voters who have already voted in every zipcode\n");
            printf("9.Press exit to exit mvote\n");
        }

        else if(strcmp(data, "exit") == 0){
            int bytes_freed = 0;     //here i store the number of all the bytes I freed
            free(line);
            bytes_freed +=MAX_SIZE*sizeof(char) + free_hashtable(Hash_Table) + free_list(List);
            free(lin);
            free(command);
            bytes_freed += 2*MAX_SIZE*sizeof(char);
            printf("%d of Bytes Released\n", bytes_freed);
            exit(0);
        }

        else{
            printf("Non existent command, press help to see all the available commands\n");
        }
    }   
    free(command);
}
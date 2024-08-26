
typedef struct List_Node{   
    int Postal_Code;
    int num_voters;   //number of voters that the list node has currently
    struct Voter_Node* voter;   //pointer to a voter node
    struct List_Node* next;     //points to the next list node
    struct List_Node* prev;     //points to the previous list node
} List_Node;

typedef struct List{
    List_Node* head;    //points to the list head node
    List_Node* tail;    //points to the list end(tail) node
    int total_voters;   //total number of voters that the list has
}List;

typedef struct Voter{
    int PIN;
    int postal_code;
    char *fname;      //first name of the voter
    char *sname;      //second/last name of the voter (or lname)
    char has_voted;
}Voter;

typedef struct Voter_Node{   //basically a list of voter nodes 
    struct Voter_Node* next;
    Voter* voter;
}Voter_Node;

typedef struct Bucket        //hash table bucket
{
    Voter **voters;          //array of pointers to voters
    int bucket_entries;      //max number of voters a bucket can contain
    struct Bucket* overflow_bucket;
    int bucket_cur_size;     //how many empty spaces does the bucket currently have
        
}Bucket;

typedef struct Hash_Table
{
    Bucket **buckets;    //array of pointers to buckets
    int num_of_keys;     //how many keys does the hash table currently have
    int hash_cur_size;   //current number of buckets
    int hash_init_size;  //number of buckets 
    int round;              
    int p;               //"pointer" to the bucket that we will split next(it just stores the place of the bucket on the hash table)
    int bucket_init_size;  //how many voters can one bucket contain (bucket_entities)
    int hash_temp_size; //to check if it is doubled
}Hash_Table;

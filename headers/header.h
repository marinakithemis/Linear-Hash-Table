#define MAX_SIZE 150   //max string size

typedef struct Hash_Table* hash_table;
typedef struct Voter* voter;
typedef struct List* list;
typedef struct Voter_Node* voter_node;
typedef struct List_Node* list_node;

struct Voter* initialize_voter(int PIN, char* fname, char* sname, int PostalCode);
struct Bucket* initialize_bucket(int bucket_entries);
struct Hash_Table* initialize_hashtable(int init_size, int bucket_entries);
struct Hash_Table* reinitialize_hashtable(struct Hash_Table *hash_table);
void hash_insert(struct Hash_Table *Hash_Table, struct Voter *voter);
int h(int i, int k, int m);
void hash_split(struct Hash_Table *Hash_Table);
void print_hash(struct Hash_Table *Hash_Table);
struct Voter* hash_search(struct Hash_Table *hash_table, int PIN);
bool is_integer (char* value);
void search_command(struct Hash_Table *hash_table, int PIN);
void mark_has_voted_command(struct Hash_Table *hash_table, struct List** list, int PIN);
int num_of_voters(struct List *list);
float voter_perc(struct Hash_Table *hash_table, struct List* list);
struct List_Node* initialize_list_node();
struct Voter_Node* initialize_voter_node(struct Voter* voter);
struct List* initialize_list();
void list_insert(struct List **list, struct Voter_Node* voter_node);
void print_list(struct List* list);
void node_swap(struct List** list, struct List_Node* node1, struct List_Node* node2);
void zipcode_print(struct List* list, int zipcode);
int free_hashtable(struct Hash_Table* hash_table);
int free_list(struct List* free_list);
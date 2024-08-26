# Linear Hash Table

This repository contains the implementation of a voting system using a linear hash table and an inverted postal code directory. Below you'll find instructions on how to run the project, along with a detailed breakdown of the implemented functions.

## How to Run

To compile the project, run the following command:

` make `

This will create the necessary executable files. The program can be executed using the following command:

`
./mvote -f <filename> -b <bucket entries> -s <hash table initial size>
`

## Linear Hash Table Implementation

### Overview
The linear hash table is implemented as a `struct`, which includes the following attributes:
- **Current Size**: The current size of the hash table.
- **Initial Size**: The initial size of the hash table.
- **Bucket Capacity**: The capacity of each bucket.
- **P**: A pointer to track the current bucket for splitting.
- **Round**: The current round of hashing.

The hash table contains an array of pointers to `bucket` structs, each of which includes:
- **Voters Array**: An array of pointers to voter structs.
- **Overflow Bucket Pointer**: A pointer to an overflow bucket.
- **Bucket Capacity**: The total capacity of the bucket.
- **Current Size**: The current number of entries in the bucket.

The `voter` struct stores information for each voter, including:
- **PIN**: Personal Identification Number.
- **First Name**.
- **Last Name** (referred to as `lname` in the code, though it's `sname` in the problem statement).
- **Postal Code**.
- **Has Voted**: A flag indicating if the voter has voted.

## Hash Table Functions

### `hash_table.c`
This file includes all the functions related to the hash table implementation:

- **Initialization Functions**: These functions dynamically allocate memory and initialize the hash table, buckets, and voters.
- **Reinitialize Hash Table**: Called during the split operation to expand the hash table by adding a new bucket. Memory is reallocated using `realloc`.
- **Hash Function**: The `h` function takes the current round (`i`), the key to hash (`k`), and the initial number of buckets (`m`) as arguments.
- **Hash Insert**: This function inserts a voter into the hash table. It checks if a bucket is full and, if so, creates an overflow bucket linked as a singly linked list. After each insertion, the `hash_split` function is called. The time complexity is `O(1)` plus the complexity of the split.
- **Hash Split**: This function splits a bucket when the load factor exceeds `0.75`. It reinitializes the bucket pointed to by `p`, re-inserts its contents using the next round's hash function, and updates the pointer `p`.
- **Print Hash**: A helper function to print the hash table, useful for debugging.
- **Hash Search**: This function searches for a voter by their PIN and returns a pointer to the voter if found, or `NULL` otherwise.
- **Search Command**: Called when the user inputs the `l` command to search for a voter.
- **Mark Has Voted Command**: Marks a voter as having voted.
- **Voter Percentage Command**: Calculates the percentage of voters who have voted.
- **Free Hash Table**: Frees all allocated memory for the hash table and its contents, returning the number of bytes freed.
- **Is Integer**: Checks if a string is an integer, used in the main function for input validation.

## Inverted Postal Code Directory Implementation

### Overview
The inverted postal code directory is implemented as a doubly linked list. Each node (`List_Node`) in the list contains:
- **Postal Code**.
- **Number of Voters**: The number of voters associated with the postal code.
- **Voter Node Pointer**: A pointer to the first voter in the list for that postal code.
- **Next Node Pointer**: A pointer to the next node in the list.
- **Previous Node Pointer**: A pointer to the previous node in the list.

The list struct (`List`) contains pointers to the first and last nodes in the list and the total number of voters.

### `list.c`
This file includes functions related to the inverted postal code directory:

- **Initialization Functions**: Dynamically allocate memory and initialize the list, list nodes, and voter nodes.
- **List Insert**: Inserts new nodes into the list. The function handles three cases:
  1. The list already contains a node with the same postal code, so the new voter is added to that node.
  2. The list is empty, so a new node is created and added as the first node.
  3. No node with the postal code exists, so a new node is created and added to the end of the list.
  
  If the position of the node needs to be adjusted due to a change in the number of voters, the `node_swap` function is called. This keeps the list sorted without requiring a full sort.

- **Free List**: Frees all allocated memory for the list and its contents.

## Main Program

### `main.c`
This file contains the main function, which:
- Validates command-line arguments and returns an error message if they are incorrect.
- Reads input data from a file using `fopen` and `strtok` to tokenize the input data.
- Processes commands in a loop until the user enters the `exit` command.
- Recognizes and handles various commands, checks input, and calls the appropriate functions.
- Includes a `help` command to display available commands to the user.
- Handles voter input with the format `<PIN> <sname> <fname> <postal_code>`.

## Additional Files

- **`header.h`**: Contains function declarations and sets the maximum string size to 150.
- **`structs.h`**: Defines all structs used for the hash table and the list.
- **`voters5000.csv`**: Contains voter entries, you can use this to test the functionality of the program.

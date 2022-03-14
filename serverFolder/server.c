#include <stdio.h>
#include <semaphore.h>

#include "insert.h"
#include "remove.h"
#include "find.h"
#include "init.h"
#include "socket.h"
#include "struct.h"
#include "hash.h"
#include "printPairs.h"
#include "server.h"

#define TABLE_SIZE 10

// Initialize nb of items
int items = 0;
int differentItems = 0;

// Mutex to manage concurrent access for insert/remove/find
extern sem_t mutex, removeMutex;

int main(int argc, char* argv[])
{
    printf("Hello from the server.c file!\n");

    // Initialize semaphore
    if (sem_init(&mutex, 0, 1) != 0) {
        fprintf(stderr, "Mutex initialization failed. Continue at your own risk...\n");
    } else {
        printf("Mutex correctly initialized\n");
    }

    // Initialize semaphore to remove items. A second mutex is needed because items can be removed from 'outside' or from 'decrement' function
    if (sem_init(&removeMutex, 0, 1) != 0) {
        fprintf(stderr, "Mutex initialization failed. Continue at your own risk...\n");
    } else {
        printf("Mutex correctly initialized\n");
    }
    
    // Create the table to store elements
    struct table *t = createTable(TABLE_SIZE);




    // Do some tests
/* ----------------------------------------------------------------------------------- */
    printf("Nb of items (expect 0): %d\n", items);
    printf("Nb of different items (exprect 0): %d\n", differentItems);

    testInsert(t);
    testHash(t);
    testRemove(t);
    testPrint(t);
    testInit(t);
    testPrint(t);
/* ----------------------------------------------------------------------------------- */



    // Start the application
    testSocket(t);
    return 0;
}

void testInsert(struct table *t)
{
    printf("\n");
    printf("INSERTION TEST\n");
    printf("Insert \"World\" without value (expect 1): %d\n", insert(t, "World"));
    printf("Insert \"Hello\" with value (expect 1): %d\n", insertWithValue(t, "Hello", 1));
    printf("Insert \"World\" without value an existing item (expect 0): %d\n", insert(t, "World"));
    printf("Insert \"UNIFR\" with value (expect 1): %d\n", insertWithValue(t, "UNIFR", 3));

    printf("Nb of items (expect 6): %d\n", items);
    printf("Nb of different items (expect 3): %d\n", differentItems);

    printf("Value of Hello (expect 1): %d\n", find(t, "Hello"));
    printf("Value of World (expect 2): %d\n", find(t, "World"));
    printf("Value of UNIFR (expect 3): %d\n", find(t, "UNIFR"));
}

void testRemove(struct table *t)
{
    printf("\n");
    printf("DELETION TEST\n");

    printf("Print all:\n");
    printAll(t);
    printf("Nb of items (expect 6): %d\n", items);
    printf("Nb of different items (expect 3): %d\n", differentItems);
    
    printf("Remove Hello (expect 0): %d\n", removeItem(t, "Hello"));

    printf("Print all:\n");
    printAll(t);

    printf("Decrement UNIFR by one (expect 0): %d\n", decrement(t, "UNIFR"));

    printf("Decrement UNIFR by ten (expect -1): %d\n", decrementWithValue(t, "UNIFR", 10)); 

    printf("Remove asdf (expect -1): %d\n", removeItem(t, "asdf"));
    printf("Remove World (expect 0): %d\n", removeItem(t, "World"));
    printf("Remove UNIFR (expect 0): %d\n", removeItem(t, "UNIFR"));

    printf("Nb of items (expect 0): %d\n", items);
    printf("Nb of different items (expect 0): %d\n", differentItems);
}

void testHash(struct table *t)
{
    printf("\n");
    printf("HASHING TEST\n");
    char shoes[] = "shoe";  
    printf("Hash code of string %s : %d\n", shoes, hashCode(t, shoes));

    char * hello = "Hello";
    printf("Hash code of string %s : %d\n", hello, hashCode(t, hello));

    char * bigHello = "HELLO";
    printf("Hash code of string %s : %d\n", bigHello, hashCode(t, bigHello));    
}

void testInit(struct table *t)
{
    int nbOfObjects = 100;
    printf("\n");
    printf("INITIALIZATION TEST\n");
    printf("Fill table (expect 0): %d\n", fillTable(t, nbOfObjects));
    printf("Nb of items (expect %d): %d\n", nbOfObjects,  items);
    printf("Nb of different items (expect ?): %d\n", differentItems);   
}

void testPrint(struct table *t)
{
    printf("\n");
    printf("PRINT TEST\n");
    printf("Print all:\n");
    printAll(t);
}

void testSocket(struct table *t) {
	printf("\n");
	printf("SERVER SOCKET TEST\n");
	createSocket(t);
}

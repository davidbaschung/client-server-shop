#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include "struct.h"
#include "insert.h"
#include "hash.h"

extern int items;
extern int differentItems;

// Mutex to manage concurrent access for insert/remove/find
extern sem_t mutex;

// Insert a new item in the hash table, or update val if it already exist
// Return 0 if update the value, 1 if create a new node
int insertWithValue(struct table *t, char* key, int val)
{
    int pos = hashCode(t, key);

    /* ----------  Enter in critical region ---------- */
    sem_wait(&mutex);

    // Find the right list
    struct node *list = t->list[pos];

    // Create a temp pointer
    struct node *temp = list;

    // Check if the key is already stored
    while(temp) 
    {
        if (strcmp(temp->key, key) == 0)
        {
            temp->value += val;
            items += val;

            /* -----------  Leave critical region ----------- */
            sem_post(&mutex);

            return 0;
        }
        temp = temp->next;
    }

    // Create a new node
    struct node *newNode = (struct node*)malloc(sizeof(struct node));

    newNode->key = malloc(sizeof(key));;
    strcpy(newNode->key, key);
    newNode->value = val;

    // Add the first node of the list as next node
    newNode->next = list;  
    // Insert at the beginning of the list 
    t->list[pos] = newNode;

    // Update count
    items += val;
    differentItems++;

    /* -----------  Leave critical region ----------- */
    sem_post(&mutex);


    return 1;
}

// If val not specify, is 1
int insert(struct table *t, char* key)
{
    return insertWithValue(t, key, 1);
}
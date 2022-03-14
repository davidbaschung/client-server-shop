#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>

#include "struct.h"
#include "remove.h"
#include "hash.h"
#include "printPairs.h"

extern int items;
extern int differentItems;

// Mutex to manage concurrent access for insert/remove/find
extern sem_t mutex;
extern sem_t removeMutex;


int decrementWithValue(struct table *t, char* key, int val)
{
    // Find postition with hashCode
    int pos = hashCode(t, key);

    /* ----------  Enter in critical region ---------- */
    sem_wait(&mutex);

    // Find the right list
    struct node *list = t->list[pos];
  
    // Create the iterator
    struct node *temp = list;

    while(temp)
    {
        if (strcmp(temp->key, key) == 0)
        {
            if (temp->value < val) {
                fprintf(stderr, "It's not possible to decrement by %d the word (%s, %d).\n", val, temp->key, temp->value);

                /* -----------  Leave critical region ----------- */
                sem_post(&mutex);
                
                return -1;
            } else if (temp->value == val) {               
                removeItem(t, key);

                /* -----------  Leave critical region ----------- */
                sem_post(&mutex); 
                return 0;
            } else {
                items -= val;
                temp->value -= val;

                /* -----------  Leave critical region ----------- */
                sem_post(&mutex);

                return 0;
            }

        }
        temp = temp->next;
    }
    
    /* -----------  Leave critical region ----------- */
    sem_post(&mutex);

    return -1;
}

int decrement(struct table *t, char* key)
{
    decrementWithValue(t, key, 1);
}


int removeItem(struct table *t, char* key)
{
    // Find position with hashCode
    int pos = hashCode(t, key);

    /* ----------  Enter in critical region ---------- */
    sem_wait(&removeMutex);

    // Find the right list
    struct node *list = t->list[pos];

    // Create the iterator
    struct node *temp = list;

    // It the list is empty
    if (!temp) {

        /* -----------  Leave critical region ----------- */
        sem_post(&removeMutex);
        
        fprintf(stderr, "Impossible to remove %s, item not present\n", key);

        return -1;
    }

    // If the key is the first item
    if(strcmp(temp->key, key) == 0) 
    {
        // Update counters
        items -= temp->value;
        differentItems--;

        // Store a pointer to the node, to free it
        struct node *deleted = temp;

        // Update reference to the first item
        t->list[pos] = temp->next;

        // Free the allocated memory
        free(deleted);

        /* -----------  Leave critical region ----------- */
        sem_post(&removeMutex);

        return 0;
    }

    // If the item is not in the firs position
    while(temp->next) 
    {
        if (strcmp(temp->next->key, key) == 0)
        {
            printf("Temp->next : (%s, %d)\n", temp->next->key, temp->next->value);
            // Update counters
            items -= temp->next->value;
            differentItems--;

            // Store a pointer to the node, to free it
            struct node *deleted = temp->next;

            temp->next = temp->next->next;

            free(deleted);

            /* -----------  Leave critical region ----------- */
            sem_post(&removeMutex);

            return 0;
        }
        temp = temp->next;
    }

    /* -----------  Leave critical region ----------- */
    sem_post(&removeMutex);

    fprintf(stderr, "Impossible to remove %s, item not present\n", key);
    return -1;
}
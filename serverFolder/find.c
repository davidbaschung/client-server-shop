#include <string.h>

#include "find.h"
#include "hash.h"

// Mutex to manage concurrent access for insert/remove/find
extern sem_t mutex;

// Find the value corresponding to the given key. Return -1 if the key is not present in the table
int find(struct table *t, char* key)
{
    int pos = hashCode(t, key);

    /* ----------  Enter in critical region ---------- */
    sem_wait(&mutex);

    // Find the right list
    struct node *list = t->list[pos];

    // Create a temp pointer
    struct node *temp = list;

    // Check if the key is stored. If so, return the value
    while(temp) 
    {
        if (strcmp(temp->key, key) == 0)
        {

            /* -----------  Leave critical region ----------- */
            sem_post(&mutex);

            return temp->value;
        }
        temp = temp->next;
    }

    /* -----------  Leave critical region ----------- */
    sem_post(&mutex);

    // If key doesn't exits, return -1
    return -1;
}
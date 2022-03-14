#ifndef OS_PROJECT_REMOVE_H
#define OS_PROJECT_REMOVE_H

#include "struct.h"

    // Decrement the val of the given key by the given value
    // If val not specify, is 1
    // Return 0 if everything went fine, -1 if the key doesn't exist
    // or if the number requested of the object is greater than the available
    // of the object already in the shop
    int decrementWithValue(struct table *t, char* key, int val);
    int decrement(struct table *t, char* key);

    // Remove the node with the given key from the table
    // Return 0 if everything went fine
    int removeItem(struct table *t, char* key);


#endif // OS_PROJECT_REMOVE_H
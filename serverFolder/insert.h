#ifndef OS_PROJECT_INSERT_H
#define OS_PROJECT_INSERT_H

#include "struct.h"

    // Insert a new item in the hash table, or update val if it already exist
    // If val not specify, is 1
    // Return 0 if update the value, 1 if create a new node
    int insertWithValue(struct table *t, char* key, int val);
    int insert(struct table *t, char* key);



#endif // OS_PROJECT_INSERT_H
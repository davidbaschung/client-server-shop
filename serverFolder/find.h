#ifndef OS_PROJECT_FIND_H
#define OS_PROJECT_FIND_H

#include "struct.h"

    // Find the value corresponding to the given key. Return -1 if the key is not present in the table
    int find(struct table *t, char* key);

#endif // OS_PROJECT_FIND_H
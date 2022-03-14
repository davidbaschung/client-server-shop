#ifndef OS_PROJECT_PRINTPAIRS_H
#define OS_PROJECT_PRINTPAIRS_H

    #include "struct.h"

    // Print all the key-value pairs in the table
    void printAll(struct table *t);

    // Print all elements in separated lists
    void printAllSeparated(struct table *t);

    // Print all the key-value pairs with the given hash (thus in the same list)
    // Return -1 if the list do not exist (if listPosition >= t->size), 
    // Return 1 if the list is empty
    // Return 0 else
    int printList(struct table *t, int listPosition);

    // Prints all the objects, keeping hidden their values
    char* printObjects(char *stringBuilder, struct table *t);

#endif // OS_PROJECT_PRINTPAIRS_H
#ifndef OS_PROJECT_STRUCT_H
#define OS_PROJECT_STRUCT_H

    #include <semaphore.h>
    
    // A node for storing items
    struct node {
        char* key; 
        int value;
        struct node *next;
        };

    // A table with the lists of nodes
    struct table {
        int size;
        struct node **list;
    };

    // Mutex to manage concurrent access for insert/remove/find
    sem_t mutex, removeMutex;
    
#endif //OS_PROJECT_STRUCT_H
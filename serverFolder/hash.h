#ifndef OS_PROJECT_HASH_H
#define OS_PROJECT_HASH_H

    // Return the hash code of the specified key, in the intervall from 0 to t->size - 1
    int hashCode(struct table *t, char* key);

#endif // OS_PROJECT_HASH_H
#include <stdio.h>
#include <string.h>
#include "printPairs.h"

// Used to indent elements without changing function signature
static char* indent = "";

// Print all the key-value pairs in the table
void printAll(struct table *t)
{
    int i;
    for (i = 0; i < t->size; i++)
    {
        printList(t, i);
    }
}

// Print all elements in separated lists
void printAllSeparated(struct table *t)
{
    // Indent the printed strings
    indent = "   ";

    int i;
    for (i = 0; i < t->size; i++)
    {
        printf("LIST %d\n", i);
        printList(t, i);
    }

    indent = "";
}

// Print all the key-value pairs with the given hash (thus in the same list)
int printList(struct table *t, int listPosition)
{
    if (listPosition >= t->size) {
        fprintf(stderr, "There are no lists in the given position\n");
        return -1;
    }
    
    // Find the right list
    struct node *list = t->list[listPosition];

    // Create a temp pointer
    struct node *temp = list;

    // Return 1 if the list is empty
    if (!temp)
        return 1;

    // Print all the elements in the list
    while (temp)
    {
        printf("%s(%s, %d)\n", indent, temp->key, temp->value);
        temp = temp->next;        
    }

    return 0;
}

char* printObjects(char* stringBuilder, struct table *t)
{  
    int empty = 0;

    for (int i = 0; i < t->size; i++)
    {
        
        if (i >= t->size) {
            fprintf(stderr, "There are no lists in the given position\n");
            return "No list at the given position\n";
        }
    
        // Find the right list
        struct node *list = t->list[i];

        // Create a temp pointer
        struct node *temp = list;

        // Return 1 if the list is empty
        if (!temp)
            empty = 1;

        // Print all the elements in the list
        while (temp && !empty)
        {
            strcat(stringBuilder, "---> ");
            strcat(stringBuilder, temp->key);
            strcat(stringBuilder, "\n");
            //printf("%s%s\n", indent, temp->key);
            temp = temp->next;        
        }
        empty = 0;
    }

    return stringBuilder;
}
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "init.h"
#include "struct.h"
#include "insert.h"

// Max length of words. Replaced if init from a file
int maxLengthOfWords = 10;

struct table *createTable(int size)
{
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    
    // Init pointers
    for (int i = 0; i < size; i++)
        t->list[i] = NULL;

    // Return the newly created table
    return t;
}

int fillTable(struct table *t, int size)
{
    // Try to add from file. If fails, add random strings
    if (addRandomWordsFromFile(t, size, "./serverFolder/words.txt") != 0)
        addRandomWords(t, size);

    return 0;
}

static char* randWord(char* word, int totalLength)
{
    int len = totalLength - 1;                          /* Start from last position in the array */
    word[len] = '\0';                                   /* Set the final symbol */

    while(len)
        word[--len] = 'a' + rand() % 26;                /* Add a random char */

    word[0] -= 32;                                      /* Capitalize first char */
        

    return word;
}

static void addRandomWords(struct table *t, int size)
{
    srand(time(NULL));                                          /* Initialize random generator */
    int length;                                                 /* Size of the word, \0 included */

    for (int i = 0; i < size; i++) {
        length = rand() % 5 + 5;
        char word[length];
        randWord(word, length);                                 /* Generate a random word */
        insert(t, word);                                        /* Insert the new word in the db */ 
    }


    char word1[10];
    char word2[5];

    /* Some tests */
    printf("Generate first random word with total length = 10: \n");
    randWord(word1, 10);
    printf("%s\n", word1);


    printf("Generate second random word with total length = 5: \n");
    randWord(word2, 5);    
    printf("%s\n", word2);

}

// Assume first line is the nb of words, the second the max length of each word (without \0)
static int addRandomWordsFromFile(struct table *t, int size, char* fileName)
{

    FILE *file = fopen(fileName, "r");      /* Open the file for read-only */
    if (file == 0) {
        fprintf(stderr, "Error in opening the file %s\n", fileName);
        return(-1);
    }
    int wordsInFile;

    /* Read the nb of words of the file, written on the first line */
    fscanf(file, "%d", &wordsInFile);

    /* Read the max length of the words, written on the second line */
    fscanf(file, "%d", &maxLengthOfWords);

    printf("There are %d words of max length %d\n", wordsInFile, maxLengthOfWords);

    if (wordsInFile == 0) {
        fprintf(stderr, "There are no words in file %s\n", fileName);
        return(-1);       
    }

    /* Consider the final \0 */
    char word[maxLengthOfWords + 1];

    /* Dynamically allocate an array for reading the words */
    char** array = malloc(wordsInFile * sizeof(char *));

    /* Read and insert first word */
    fscanf(file, "%s", word);


    /* Read and insert until there are words left */
    int i = 0;
    while (!feof(file)) {
        array[i] = malloc((maxLengthOfWords + 1) * sizeof(char*));
        strcpy(array[i], word);
        fscanf(file, "%s", word);
        i++;
    }


    /* Take random items from array and insert in the table */
    insertFromArray(t, size, array, wordsInFile);

    /* Free the allocated memory */
    for (int j = 0; j < wordsInFile; j++)
        free(array[j]);

    free(array);


    /* Close the file */
    fclose(file);

    return 0;
}

// After the initialization of the array, pick random elements and insert in the table */
static void insertFromArray(struct table *t, int size, char** array, int wordsInFile)
{
    srand(time(NULL));                                          /* Initialize random generator */

    // Insert the requested nb of elements
    for (int i = 0; i < size; i++) {
        char * newWord = array[rand() % wordsInFile];
        insert(t, newWord);
//        printf("Inserting new word: %s\n", newWord);
    }
}


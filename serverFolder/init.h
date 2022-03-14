#ifndef OS_PROJECT_INIT_H
#define OS_PROJECT_INIT_H

	// Create a new hash table
	struct table *createTable(int size);

	// Insert random initial elements
	// Return 0 if everythig went fine
	int fillTable(struct table *t, int size);

	static char* randWord(char* word, int totalLength);
	static void addRandomWords(struct table *t, int size);
	static int addRandomWordsFromFile(struct table *t, int size, char* fileName);
	static void insertFromArray(struct table *t, int size, char** array, int wordsInFile);

#endif //OS_PROJECT_INIT_H

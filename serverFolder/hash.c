#include <stdlib.h>
#include <stdio.h>

#include "struct.h"

int hashCode(struct table *t, char* key){

	int M = t->size;
	int calc = 0;

	// Horner's method for a more uniform hashing
	const int c = 31;									/* Prime constant for the hashing */

	for (char* temp = key; *temp != '\0'; temp++)		/* Iterate over each char */
		calc = (*temp + (calc * c)) % M;				
	
	
	return calc;
}

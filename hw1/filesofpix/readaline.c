/* 
 *   readaline.c
 *   by: Drew Maynard and Joel Brandinger, 01/27/21
 *   Files, Pictures and Interfaces
 *
 *   This file is used to read bytes from a pgm file and store them
 *   in an array until a '\n' character is reached. The readaline function
 *   will return the number bytes in the array and will have the pointer
 *   passed in as the argument point to the first byte in the array.
*/

#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "uncorrupt.h"
#include <atom.h>
#include <list.h>
#include <table.h>
#include <except.h>
#include <assert.h>

/* readaline
 *
 *    Purpose: Read in a full line of characters for restoration to store
 * Parameters: The input file stream and a reference to a pointer
 *    Returns: Number of bytes in the line
 *
*/
size_t readaline(FILE *inputfd, char **datapp)
{
    assert(inputfd != NULL);
    assert(datapp != NULL);
    
    char currChar = getc(inputfd);
    int size = 100;

    if (currChar == EOF) {
        *datapp = NULL;
        return 0;
    }
    char *array = (char*) malloc(size * sizeof(char));
    assert(array != NULL);
    int numBytes = 0;

    while (currChar != 10) {
        if (numBytes == size - 1) {
            size = size * 2;
            array = (char*) realloc(array, size * sizeof(char));
            assert(array != NULL);
        }

        array[numBytes] = currChar;
        numBytes++;
        currChar = getc(inputfd);
    }

    array[numBytes] = '\0';
    array = (char*) realloc(array, (1 + numBytes) * sizeof(char));
    assert(array != NULL);
    *datapp = array;
    
    size_t result = numBytes;
    return result;
}


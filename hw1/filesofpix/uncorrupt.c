/* 
 *   uncorrupt.c
 *   by: Drew Maynard and Joel Brandinger, 01/27/21
 *   Files, Pictures and Interfaces
 *
 *   This file will take in the lines of the corrupted image, proccess
 *   them, and give back the uncorrupted lines.
*/

#include "readaline.h"
#include "uncorrupt.h"
#include <stdlib.h>
#include <stdio.h>
#include <atom.h>
#include <list.h>
#include <table.h>
#include <stdbool.h>
#include <string.h>
#include "seq.h"
#include <assert.h>

/* uncorrupt()
 *
 *    Purpose: Perform all functions to uncorrupt an image
 * Parameters: A pointer to the input file
 *    Returns: None
*/
void uncorrupt(FILE *fp)
{
    char *datapp;
    Table_T table = Table_new(100, NULL, NULL);
    Seq_T lines = Seq_new(0);
    char *key = NULL;
    
    int bytes = readaline(fp, &datapp);
    hash_to_table(datapp, &table, bytes, &lines, &key);

    while (datapp != NULL) {
        int bytes = readaline(fp, &datapp);
        hash_to_table(datapp, &table, bytes, &lines, &key);
    }

    getLastElement(&table, &lines, key); 
    int numRows = Seq_length(lines);
    printList(lines, numRows, Seq_length(Seq_get(lines, 0)));

    free(key);
    freeSequence(lines);
    Table_map(table, tFree, NULL);
    Table_free(&table); 
}

/* hash_to_table()
 *
 *    Purpose: Hashes the sequence from corrupted image into the table
 * Parameters: A pointer to the line of characters and the table
 *    Returns: None
*/
void hash_to_table(char *datapp, Table_T *table, int bytes, Seq_T *list, \
                   char **key)
{
    if (datapp == NULL) {
        return;
    }
    
    Seq_T numbers = Seq_new(0);
    char *characters = (char*)malloc(bytes * sizeof(char));
    assert(characters != NULL); 
    int a = processLine(bytes, datapp, &numbers, characters);
    
    const char *sequence;
    sequence = Atom_string(characters);

    if (Table_get(*table, sequence) != NULL) {
        Seq_addhi(*list, Table_put(*table, sequence, numbers));
        if (*key == NULL) {
            *key = copyCharArray(characters, a + 1);
        }
    } else {
        Table_put(*table, sequence, numbers);
    }
    free(datapp);
    free(characters);
}

/* proccessLine
 *
 *    Purpose: Uses the line given and splits the characters into two lists:
               One containing the characters in the line and the other
               finds the characters that are integers, converts them to int,
               and adds them to the list.
 * Parameters: The number of characters in the line, a pointer to the line,
               and the addresses of the char and int lists
 *    Returns: The number of characters in the char list
*/
int processLine(int bytes, char *datapp, Seq_T *numbers, char *characters)
{
    char currChar;
    int a = 0;

    for (int i = 0; i < bytes; i++) {
        currChar = datapp[i];
        if (!isDigit(currChar)) {
            characters[a] = currChar;
            a++;
        } else {
            int x = currChar - 48;
            int counter = 1;
            while (isDigit(datapp[i + counter])) {
                currChar = datapp[i + counter];
                x = x * 10 + (currChar - 48);
                counter++;
            }
            i = i + counter - 1;
            int *val = (int*) malloc(sizeof(int));
            assert(val != NULL);
            *val = x;
            Seq_addhi(*numbers, val);
        }
    } 

    characters[a] = '\0';
    return a;
}

/* isDigit
 *
 *    Purpose: Checks to see if a character is a digit in ascii or not
 * Parameters: The character
 *    Returns: True if it is a digit
*/
bool isDigit(char c)
{
    if ((c < 48) || (c > 57)) {
        return false;
    }
    return true;
}

/* copyCharArray
 *
 *    Purpose: Add values of existing array to an array with space on the
 *             heap.
 * Parameters: An array to copy and a reference to a pointer with the space
 *    Returns: None, will update arrays values.
*/
char *copyCharArray(char *array, int memorySize)
{
    char *newArray = (char*) malloc(memorySize * sizeof(char));
    assert(newArray != NULL);
    memcpy(newArray, array, memorySize * sizeof(char));
    assert(newArray != NULL);
    
    return newArray;
}

/* printToCout
 *
 *    Purpose: Print the contents of the list in P5 format
 * Parameters: The list, the number of rows, number of columns to print
 *    Returns: None
*/
void printList(Seq_T list, int numRows, int numColumns)
{
    printf("%s\n%d %d\n%d\n", "P5", numColumns, numRows, 255);

    for (int i = 0; i < numRows; i++) {
        printIntSequence(Seq_get(list, i), numColumns);
    }
}

/* print
 *
 * Helper Function for printToCout
 *
 *    Purpose: Prints the elements of the arrays
 * Parameters: The array and the size of each array
 *    Returns: None
*/
void printIntSequence(Seq_T seq, int size)
{
    for (int i = 0; i < size; i++) {
        printf("%c", *(int*)Seq_get(seq, i));
    }
}

/* getLastElement
 *
 *    Purpose: Gets the last element in the table that was left and adds
 *             to list
 * Parameters: The table, the list, and the sequence to check
 *    Returns: None
*/
void getLastElement(Table_T *table, Seq_T *list, char *sequence)
{
    const char *key = Atom_string(sequence);
    Seq_addhi(*list, Table_get(*table, key));
    Table_remove(*table, key);
}

/* tFree
 *
 *    Purpose: Dealocate the memory in the table
 * Parameters: The key and value pairs
 *    Returns: None
*/
void tFree(const void *key, void **value, void *cl)
{
    Seq_T *seq = (Seq_T*)value;
    freeNumbers(*seq);
    (void)key;
    (void)cl;
}

/* freeSequence
 *
 *    Purpose: Dealocate the memory in the sequence of sequences
 * Parameters: The sequence
 *    Returns: None
*/
void freeSequence(Seq_T seq)
{
    for (int i = 0; i < Seq_length(seq); i++) {
        freeNumbers(Seq_get(seq, i));
    }
    Seq_free(&seq);
}

/* freeNumbers
 *
 *    Purpose: Dealocate the numbers in the inner sequence
 * Parameters: The sequence
 *    Returns: None
*/
void freeNumbers(Seq_T seq)
{
    for (int i = 0; i < Seq_length(seq); i++) {
        free((int*)Seq_get(seq, i));
    }
    Seq_free(&seq);
}


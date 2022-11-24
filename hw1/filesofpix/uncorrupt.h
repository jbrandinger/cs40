/* 
 *   uncorrupt.h
 *   by: Drew Maynard and Joel Brandinger, 01/27/21
 *   Files, Pictures and Interfaces
 *
 *   This file contains the interface for the uncorrupt program
*/

#include <atom.h>
#include <list.h>
#include <table.h>
#include <stdbool.h>
#include <stdlib.h>
#include <seq.h>

void uncorrupt(FILE *fp);
void hash_to_table(char *datapp, Table_T *table, int bytes, Seq_T *list,
                   char **key);
int processLine(int bytes, char *datapp, Seq_T *numbers, char *characters);
bool isDigit(char c);
void printList(Seq_T list, int numRows, int numColumns);
void printIntSequence(Seq_T seq, int size);
void getLastElement(Table_T *table, Seq_T *list, char *sequence);
void freeList(List_T list, int size);
char *copyCharArray(char *array, int memorySize);
void tFree(const void *key, void **value, void *cl);
void freeSequence(Seq_T seq);
void freeNumbers(Seq_T seq);

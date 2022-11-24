/* 
 *   restoration.c
 *   by: Drew Maynard and Joel Brandinger, 01/27/21
 *   Files, Pictures and Interfaces
 *
 *   This file is used to perform operations and run other programs to
 *   uncorrupt and transform the corrupted P2 into a usable P5.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readaline.h"
#include "uncorrupt.h"
#include <assert.h>

FILE *openFile(char *filename);
void printString(char *string);

int main(int argc, char **argv)
{
    assert(argc < 3);
    
    if (argc == 1) {
        char filename[1000];
        printf("%s", "Enter file to restore: ");
        scanf("%s", filename);
        FILE *fp = openFile(filename);
        uncorrupt(fp);
        fclose(fp);
        return EXIT_SUCCESS;
    } else {
        FILE *fp = openFile(argv[1]);
        uncorrupt(fp);
        fclose(fp);
        return EXIT_SUCCESS;
    } 

    return 0;
}

/* openFile
 *
 *    Purpose: Open a file for the user and check for success
 * Parameters: The name of the file and the name of the program
 *    Returns: A pointer to the file stream that was opened
 *
*/
FILE *openFile(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    assert(fp != NULL);
    return fp;
}


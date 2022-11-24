/* unblackedges.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/09/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This program takes in an image in pbm format and removes the black
 * edges.
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fixEdge.h"

FILE *openFile(char *filename, char *program);

int main(int argc, char **argv)
{
    if (argc == 1) {
    char filename[1000];
    scanf("%s", filename);
    FILE *fp = openFile(filename, argv[0]);
    fixEdge(fp);
    fclose(fp);
    } else if (argc == 2) {
        FILE *fp = openFile(argv[1], "rb");
        fixEdge(fp);
        fclose(fp);
    } else {
        fprintf(stderr, "Too many arguments.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

/* openFile
 *
 *    Purpose: Open a file for the user and check for success
 * Parameters: The name of the file and the name of the program
 *    Returns: A pointer to the file stream that was opened
 *
*/
FILE *openFile(char *filename, char *program)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
            fprintf(stderr, "%s: %s %s %s\n",
                    program, "Could not open file",
                    filename, "for reading.");
             exit(EXIT_FAILURE);
    } else {
        return fp;
    }
}


















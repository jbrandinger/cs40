/* stack.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/14/22
 * Interfaces, Implementations, and Images (iii)
 *
 * Program takes in a correctly formated pgm file that contains a 9 by 9
 * graymap that represents a sudoku board and will return with exit
 * success if the board is a vaild solved board. The pixel intensities
 * represent the different numbers on sudoku board.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray2.h"
#include "solved.h"

FILE *openFile(char *filename, char *program);
UArray2_T pgmread(FILE *inputfp);
void placeInt(int i, int j, UArray2_T array, void *val, void *cl);
void print(int i, int j, UArray2_T array, void *val, void *p1);

int main(int argc, char **argv)
{
    if (argc == 1) {
        char filename[1000];
        scanf("%s", filename);
        FILE *fp = openFile(filename, argv[0]);
        bool solved = isSolved(fp);
        fclose(fp);
        if (solved) {
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    } else if (argc == 2) {
        FILE *fp = openFile(argv[1], "rb");
        bool solved = isSolved(fp);
        fclose(fp);
        if (solved) {
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Too many arguments.\n");
        exit(EXIT_FAILURE);
    }
}

/* openFile
 *
 *    Purpose: Open a file for the user and check for success
 *
 * Parameters: The name of the file and the name of the program
 *
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















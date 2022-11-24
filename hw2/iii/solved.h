/* solved.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/14/22
 * Interfaces, Implementations, and Images (iii)
 *
 * Program will check to see if the given sudoku board is solved. It assumes
 * the given board is in correct format.
*/

#include <stdbool.h>
#include <stdlib.h>
#include "uarray2.h"

bool isSolved(FILE *inputfp);
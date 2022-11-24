/* solved.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/14/22
 * Interfaces, Implementations, and Images (iii)
 *
 * File contains the interface for the solved program.
*/

#include "solved.h"
#include <pnmrdr.h>

bool checkComponents(UArray2_T array, int lBound, int rBound, int upBound, \
        int lowBound);
void placeInt(int i, int j, UArray2_T array, void *val, void *cl);
UArray2_T pgmread(FILE *inputfp);

/* isSolved
 *
 *      Purpose: Checks all conditions of a winning sudoku board.
 *
 *   Parameters: The array representing the board.
 *
 *      Returns: True if the board is valid.
 *
 * Expectations: None
 *
*/
bool isSolved(FILE *inputfp)
{
    UArray2_T array = pgmread(inputfp);
    assert(array);

    bool OK = true;

    /* rows */
    for (int i = 0; i < 9; i++) {
        OK = checkComponents(array, 0, 8, i, i);
        if (!OK) {
            UArray2_free(&array);
            return false;
        }
    }

    /* columns */
    for (int i = 0; i < 9; i++) {
        OK = checkComponents(array, i, i, 0, 8);
        if (!OK) {
            UArray2_free(&array);
            return false;
        }
    }

    /* boxes */
    for (int i = 0; i < 9; i+= 3) {
        for (int j = 0; j < 9; j+= 3) {
            OK = checkComponents(array, j, j + 2, i, i + 2);
            if (!OK) {
                UArray2_free(&array);
                return false;
            }
        }
    }
    UArray2_free(&array);
    return OK;
}

/* pgmread
 *
 *      Purpose: Read in the file and make create the 2D array representing
 *               the board.
 *
 *   Parameters: The input file stream
 *
 *      Returns: The 2D array with the values filled in.
 *
 * Expectations: Making sure the file is a correctly formated pgm image.
*/
UArray2_T pgmread(FILE *inputfp)
{
    Pnmrdr_T image = Pnmrdr_new(inputfp);
    Pnmrdr_mapdata data = Pnmrdr_data(image);
    assert(data.type == 2);
    assert(data.width == 9);
    assert(data.height == 9);
    assert(data.denominator == 9);
    UArray2_T array = UArray2_new(data.width, data.height, sizeof(int));
    UArray2_map_row_major(array, placeInt, image);
    Pnmrdr_free(&image);
    
    return array;
}

/* placeInt
 *
 *      Purpose: Function is made to be used as the apply functions when
 *               mapping the Uarray2 in row major order. Places a integer in 
 *               the correct location.
 *
 *   Parameters: The column, row, Uarray2 instance, the value to place, and the
 *               instance of the Pnmrdr reader that reads the bits from input.
 *
 *      Returns: None.
 *
 * Expectations: cl is a pnmrdr instance and array is a Uarray2 instance.
*/
void placeInt(int i, int j, UArray2_T array, void *val, void *cl)
{
    int element = Pnmrdr_get(cl);
    assert(element <= 9);
    assert(element != 0);
    *(int*)val = element;
    (void)i;
    (void)j;
    (void)array;
}

/* checkComponents
 *
 *      Purpose: Check to make sure each component in board is valid.
 *
 *   Parameters: The array, the left and right bounds, the upper and lower
 *               bounds.
 *
 *      Returns: True if the board is valid.
 *
 * Expectations: None
 *
*/
bool checkComponents(UArray2_T array, int lBound, int rBound, int upBound, \
        int lowBound)
{
    int iterator = lBound;

    /* Table uses 0 based indexing, so hashing value 1 goes into index 0 */
    int table[9];

    for (int i = 0; i < 9; i++) {
        table[i] = 0;
    }

    while (upBound <= lowBound) {
        table[*(int*)UArray2_at(array, iterator, upBound) - 1] = 1;
        iterator++;
        if (iterator > rBound) {
            iterator = lBound;
            upBound++;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (table[i] == 0) {
            return false;
        }
    }

    return true;
}






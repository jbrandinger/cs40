/* bit2.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the implementation for the bit2 data structure
 *
*/

#include <stdlib.h>
#include "bit2.h"

#define T Bit2_T

struct T {
    int width;
    int height;
    Bit_T array;
};

/* Bit2_new
 *
 *      Purpose: Allocate, initialize, and return a new 2-dimensional array
 *               from the specified width and height. Each element in the 2D
 *               array occupies one bit for size. Uses zero based indexing. 2D
 *               array is implemented as a one dimensional array in row major
 *               order.
 *
 *   Parameters: The width and height where width represents the number of
 *               columns and height represents the number of rows. Size
 *               represents the number of bytes allocated by each element.
 *
 *      Returns: A pointer to the UArray2_T struct.
 *
 * Expectations: Memory allocation is successful, size of structure is correct,
 *               Built in UArray class handles most exceptions.
 *
*/
extern T Bit2_new(int width, int height)
{
    assert(width >= 0);
    assert(height >= 0);
    T new_array = (void*)malloc(sizeof(struct T));
    assert(new_array != NULL);
    new_array->width = width;
    new_array->height = height;
    new_array->array = Bit_new(width * height);

    return new_array;
}

/* Bit2_free
 *
 *      Purpose: Deallocates and clears any memory associated with the Bit2
 *             instance.
 *
 *   Parameters: The pointer to the instance of Bit2.
 *
 *      Returns: None
 *
 * Expectations: bit array is not null.
 *
*/
extern void Bit2_free(T *bit2)
{
    assert(*bit2);
    Bit_free(&(*bit2)->array);
    free(*bit2);
}

/* Bit2_width
 *
 *      Purpose: Return the width of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of Bit2.
 *
 *      Returns: The width of the array.
 *
 * Expectations: Bit is not null.
 *
*/
extern int Bit2_width(T bit2)
{
    assert(bit2);
    return bit2->width;
}

/* Bit2_height
 *
 *      Purpose: Return the height of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of Bit2.
 *
 *      Returns: The height of the array.
 *
 * Expectations: Bit array is not null.
 *
*/
extern int Bit2_height(T bit2)
{
    assert(bit2);
    return bit2->height;
}





/* Bit2_get
 *
 *      Purpose: Access the element stored at a given column and row index.
 *
 *   Parameters: The pointer to the instance of Bit2, the column number,
 *               and the row number.
 *  
 *      Returns: The bit stored at the specified 2 dimensional index.
 *
 * Expectations: The given index is within the bounds of the 2D array, bit2
 *               is not null.
 *
*/
extern int Bit2_get(T bit2, int col, int row)
{
    assert(bit2);
    assert(col < bit2->width);
    assert(row < bit2->height);
    assert(col >= 0 && row >= 0);
 
    int index = col + (row * bit2->width);
    return Bit_get(bit2->array, index);
}







/* Bit2_put
 *
 *      Purpose: Places a bit at a given column and row index.
 *
 *   Parameters: The pointer to the instance of Bit2, the column number,
 *               and the row number.
 *  
 *      Returns: The previous value stored at that bit index.
 *
 * Expectations: The given index is within the bounds of the 2D array, bit2
 *               is not null.
 *
*/
extern int Bit2_put(T bit2, int col, int row, int bit)
{
    assert(bit2);
    assert(col < bit2->width);
    assert(row < bit2->height);
    assert(col >= 0 && row >= 0);
 
    int index = col + (row * bit2->width);
    return Bit_put(bit2->array, index, bit);
}

/* Bit2_map_row_major
 *
 *      Purpose: Traverse each element in 2D array in row major order.
 *
 *   Parameters: The instance of Bit2, the function to be applied to each
 *               element, the folding variable if needed.
 * 
 *      Returns: None.
 *
 * Expectations: Bit array is not null.
 *
*/
extern void Bit2_map_row_major(T bit2, void (apply(int i, int j, T bit2,
    int bit, void *cl)), void *cl)
{
    assert(bit2);
    for (int j = 0; j < bit2->height; j++) {
        for (int i = 0; i < bit2->width; i++) {
             int index = i + (j * bit2->width);
             apply(i, j, bit2, Bit_get(bit2->array, index), cl);
        }
    }
}

/* Bit2_map_col_major
 *
 *      Purpose: Traverse each element in 2D array in column major order.
 *
 *   Parameters: The instance of Bit2, the function to be applied to each
 *               element, the folding variable if needed.
 * 
 *      Returns: None.
 *
 * Expectations: Bit array is not null.
 *
*/
extern void Bit2_map_col_major(T bit2, void (apply(int i, int j, T bit2,
    int bit, void *cl)), void *cl)
{
    assert(bit2);
    for (int i = 0; i < bit2->width; i++) {
        for (int j = 0; j < bit2->height; j++) {
             int index = i + (j * bit2->width);
             apply(i, j, bit2, Bit_get(bit2->array, index), cl);
        }
    }
}



























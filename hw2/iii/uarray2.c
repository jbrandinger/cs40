/* uarray2.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the implementation for the uarray2 data type
 *
*/

#include "uarray2.h"
#include <stdlib.h>

#define T UArray2_T

struct T {
    int width;
    int height;
    int size;
    UArray_T array;
};

/* UArray2_new
 *
 *      Purpose: Allocate, initialize, and return a new 2-dimensional array
 *               from the specified width and height. Each element in the 2D
 *               array occupies "size" bytes. Uses zero based indexing. 2D
 *               array is implemented as a one dimensional array in row major
 *               order.

 *   Parameters: The width and height where width represents the number of
 *               columns and height represents the number of rows. Size
 *               represents the number of bytes allocated by each element.
 *
 *      Returns: A pointer to the UArray2_T struct.
 *
 * Expectations: Memory allocation is successful, size of structure is 
 *               correct, built in UArray class handles most exceptions.
 *
*/
extern T UArray2_new(int width, int height, int size)
{
    assert(width >= 0);
    assert(height >= 0);
    T new_array = (void*)malloc(sizeof(struct T));
    assert(new_array != NULL);
    new_array->width = width;
    new_array->height = height;
    new_array->size = size;
    new_array->array = UArray_new(width * height, size);

    return new_array;
}

/* UArray2_free
 *
 *      Purpose: Deallocates and clears any memory associated with the UArray2
 *               instance.
 *
 *   Parameters: The pointer to the instance of UArray2.
 *
 *      Returns: None
 *
 * Expectations: uarray is not null.
 *
*/
extern void UArray2_free(T *uarray2)
{
    assert(*uarray2);
    UArray_free(&(*uarray2)->array);
    free(*uarray2);
}


/* UArray2_width
 *
 *      Purpose: Return the width of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of UArray2.
 *
 *      Returns: The width of the array.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2_width(T uarray2)
{
    assert(uarray2);
    return uarray2->width;
}

/* UArray2_height
 *
 *      Purpose: Return the height of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of UArray2.
 *
 *      Returns: The height of the array.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2_height(T uarray2)
{
    assert(uarray2);
    return uarray2->height;
}

/* UArray2_size
 *
 *      Purpose: Return the size of each element in bytes.
 *
 *   Parameters: The pointer to the instance of UArray2.
 *
 *      Returns: The number of bytes each element can hold.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2_size(T uarray2)
{
    assert(uarray2);
    return uarray2->size;
}

/* UArray2_at
 *
 *      Purpose: Access the element stored at a given column and row index.
 *
 *   Parameters: The pointer to the instance of UArray2, the column number,
 *               and the row number.
 *  
 *      Returns: A void pointer to the element being stored at the specified
 *               2 dimensional index.
 *
 * Expectations: The given index is within the bounds of the 2D array, Uarray
 *               is not null.
 *
*/
void *UArray2_at(T uarray2, int col, int row)
{
    assert(uarray2);
    assert(col < uarray2->width);
    assert(row < uarray2->height);
    assert(col >= 0 && row >= 0);
 
    int index = col + (row * uarray2->width);
    return UArray_at(uarray2->array, index);
}

/* UArray2_map_row_major
 *
 *      Purpose: Traverse each element in 2D array in row major order.
 *
 *   Parameters: The instance of Uarray2, the function to be applied to each
 *               element, the folding variable if needed.
 * 
 *      Returns: None.
 *
 * Expectations: Uarray is not null.
 *
*/
void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
    T uarray2, void *val, void *cl), void *cl)
{
    assert(uarray2);
    for (int j = 0; j < uarray2->height; j++) {
        for (int i = 0; i < uarray2->width; i++) {
             int index = i + (j * uarray2->width);
             apply(i, j, uarray2, UArray_at(uarray2->array, index), cl);
        }
    }
}

/* UArray2_map_column_major
 *
 *      Purpose: Traverse each element in 2D array in column major order.
 *
 *   Parameters: The instance of Uarray2, the function to be applied to each
 *               element, the folding variable if needed.
 *
 *      Returns: None.
 *
 * Expectations: UArray is not null.
 *
*/
void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
    T uarray2, void *val, void *cl), void *cl)
{
    assert(uarray2);
    for (int i = 0; i < uarray2->width; i++) {
        for (int j = 0; j < uarray2->height; j++) {
             int index = i + (j * uarray2->width);
             apply(i, j, uarray2, UArray_at(uarray2->array, index), cl);
        }
    }
}

#undef T










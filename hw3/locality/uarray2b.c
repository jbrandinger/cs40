/* 
 *   uarray2b.c
 *   by: Drew Maynard and Joel Brandinger, 02/21/22
 *   Locality
 *
 *   This file holds the implementation for the uarray2b class. This class
 *   uses methods from multiple implementations to create one working 2D
 *   array class.
*/

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "uarray2b.h"
#include "uarray2.h"

#define T UArray2b_T

struct T {
    int width;
    int height;
    int blocksize;
    int size;
    UArray2_T array;
};

void buildBlocks(int i, int j, UArray2_T array, void *val, void *cl);

/* UArray2b_new
 *
 *      Purpose: Initialize and create a new blocked 2D array with the
 *               correct specifications.
 *
 *   Parameters: The width (x axis), the height (y axis), the size of each
 *               element in each cell, and the number of cells on one
 *               side in each block.
 *
 *      Returns: The blocked array
 *
 * Expectations: width, height, and size are all non negative. Blocksize of
 *               each element is at least 1. Make sure memory is
 *               allocated successfully.
*/
extern T UArray2b_new(int width, int height, int size, int blocksize)
{
    assert(width >= 0);
    assert(height >= 0);
    assert(size >= 0);
    assert(blocksize >= 1);
    T new_array = (void*)malloc(sizeof(struct T));
    assert(new_array != NULL);

    int newWidth = width / blocksize;
    int newHeight = height / blocksize;

    if (width % blocksize != 0) {
        newWidth++;
    }

    if (height % blocksize != 0) {
        newHeight++;
    } 

    new_array->width = width;
    new_array->height = height;
    new_array->size = size;
    new_array->blocksize = blocksize;
    new_array->array = UArray2_new(newWidth, newHeight, 
        blocksize * blocksize * size);

    UArray2_map_row_major(new_array->array, buildBlocks, new_array);

    return new_array;
}

/* buildBlocks
 *
 *      Purpose: Map the newly crated UArray2 and place a regular Uarray in
 *               each element that will act as the blocks.
 *
 *   Parameters: The column, row, Uarray2 instance, the value to place, 
 *    and the instance of the Pnmrdr reader that reads the bits from input.
 *
 *      Returns: None.
 *
 * Expectations: None.
*/
void buildBlocks(int i, int j, UArray2_T array, void *val, void *cl)
{
    int numCells = UArray2b_blocksize(cl) * UArray2b_blocksize(cl);   
    *(UArray_T*)UArray2_at(array, i, j) = UArray_new(numCells, 
        UArray2b_size(cl));
    (void)val;
}

/* UArray2b_new_64K_block
 *
 *      Purpose: Initialize and create a new blocked 2D array with blocksize
 *               as large as possible (MAX 64 KB)
 *
 *   Parameters: The width (x axis), the height (y axis), the size of each
 *               element in each cell.
 *
 *      Returns: The blocked array
 *
 * Expectations: width and height are nonegative. Size of
 *               each element is greater than 0. Make sure memory is
 *               allocated successfully.
*/
extern T UArray2b_new_64K_block(int width, int height, int size)
{
    int maxBlock = 64 * 1028;
    if (size > maxBlock) {
        return UArray2b_new(width, height, size, 1);
    } else {
        int blocksize = sqrt(maxBlock / size);
        return UArray2b_new(width, height, size, blocksize);
    }
}

/* UArray2b_free
 *
 *      Purpose: Deallocates and clears any memory associated with the
 *               UArray2b instance.
 *
 *   Parameters: The pointer to the instance of UArray2b.
 *
 *      Returns: None
 *
 * Expectations: uarray is not null.
 *
*/
extern void UArray2b_free(T *array2b)
{
    assert(*array2b);

    for (int i = 0; i < UArray2_height((*array2b)->array); i++) {
        for (int j = 0; j < UArray2_width((*array2b)->array); j++) {
            UArray_free(UArray2_at((*array2b)->array, j, i));
        }
    }

    UArray2_free(&(*array2b)->array);
    free(*array2b);
}

/* UArray2b_width
 *
 *      Purpose: Return the width of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of UArray2b.
 *
 *      Returns: The width of the array.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2b_width(T array2b)
{
    assert(array2b);
    return array2b->width;
}

/* UArray2b_height
 *
 *      Purpose: Return the height of the 2 dimensional array.
 *
 *   Parameters: The pointer to the instance of UArray2b.
 *
 *      Returns: The height of the array.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2b_height(T array2b)
{
    assert(array2b);
    return array2b->height;
}

/* UArray2b_size
 *
 *      Purpose: Return the size of each element in bytes.
 *
 *   Parameters: The pointer to the instance of UArray2b.
 *
 *      Returns: The number of bytes each element can hold.
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2b_size(T array2b)
{
    assert(array2b);
    return array2b->size;
}

/* UArray2b_blocksize
 *
 *      Purpose: Return the width of one block
 *
 *   Parameters: The pointer to the instance of UArray2b.
 *
 *      Returns: Blocksize represented as the width of one block
 *
 * Expectations: Uarray is not null.
 *
*/
extern int UArray2b_blocksize(T array2b)
{
    assert(array2b);
    return array2b->blocksize;
}

/* UArray2B_at
 *
 *      Purpose: Access the element stored at a given column and row index.
 *
 *   Parameters: The pointer to the instance of UArray2B, the column number,
 *               and the row number.
 *  
 *      Returns: A void pointer to the element being stored at the specified
 *               2 dimensional index.
 *
 * Expectations: The given index is within the bounds of the 2D array, Uarray
 *               is not null.
 *
*/
extern void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b);
    assert(column < array2b->width);
    assert(row < array2b->height);
    assert(column >= 0 && row >= 0);
    int blocksize = array2b->blocksize;

    UArray_T *block = UArray2_at(array2b->array, column / blocksize,
     row / blocksize);

    return UArray_at(*block, blocksize * (row % blocksize) + 
        (column % blocksize));
}

/* UArray2b_map
 *
 *      Purpose: Traverse each element in 2D array in block major order.
 *
 *   Parameters: The instance of Uarray2, the function to be applied to each
 *               element, the folding variable if needed.
 * 
 *      Returns: None.
 *
 * Expectations: Uarray is not null.
 *
*/
extern void  UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                                     void *elem, void *cl), void *cl)
{
    assert(array2b);
    int cellsPerBlock = array2b->blocksize * array2b->blocksize;

    for (int j = 0; j < UArray2_height(array2b->array); j++) {
        for (int i = 0; i < UArray2_width(array2b->array); i++) {
             for (int x = 0; x < cellsPerBlock; x++) {
                int col = (x % array2b->blocksize) + (i * array2b->blocksize);
                int row = (x / array2b->blocksize) + (j * array2b->blocksize);

                if (col < array2b->width && row < array2b->height) {
                    apply(col, row, array2b, 
                      UArray_at(*(UArray_T*)
                        UArray2_at(array2b->array, i, j), x), cl);
                }
             }
        }
    }
}






















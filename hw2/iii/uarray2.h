/* uarray2.h
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the interface for the uarray2 data type
 *
*/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdio.h>

#include "uarray.h"
#include "assert.h"

#define T UArray2_T
typedef struct T *T;

extern T UArray2_new(int width, int height, int size);
extern void UArray2_free(T *uarray2);

extern int UArray2_width(T uarray2);
extern int UArray2_height(T uarray2);
extern int UArray2_size(T uarray2);
void *UArray2_at(T uarray2, int col, int row);

void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
    T uarray2, void *val, void *cl), void *cl);
void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
    T uarray2, void *val, void *cl), void *cl);

#undef T
#endif


/* bit2.h
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the interface for the bit2 data structure.
 *
*/

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#include "assert.h"
#include <bit.h>

#define T Bit2_T
typedef struct T *T;

extern T Bit2_new(int width, int height);
extern void Bit2_free(T *bit2);

extern int Bit2_width(T bit2);
extern int Bit2_height(T bit2);

extern int Bit2_get(T bit2, int col, int row);
extern int Bit2_put(T bit2, int col, int row, int bit);

extern void Bit2_map_row_major(T bit2, void (apply(int i, int j, T bit2,
    int bit, void *cl)), void *cl);
extern void Bit2_map_col_major(T bit2, void (apply(int i, int j, T bit2,
    int bit, void *cl)), void *cl);

#undef T 
#endif
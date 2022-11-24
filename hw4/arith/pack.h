/* pack.h
 *
 * header file for packing all values into code word 
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "bigE.h"

// move to .c file 
struct bits {
    unsigned PbChroma;
    unsigned PrChroma;
    unsigned a;
    signed b;
    signed c;
    signed d;
};

A2Methods_UArray2 packBits(A2Methods_UArray2 quantArray);
A2Methods_UArray2 unpackBits(A2Methods_UArray2 wordArray);

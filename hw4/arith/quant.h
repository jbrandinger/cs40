/* quant.h
 *
 * header file for quantizing
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "pack.h"

// move to .c file 
struct coefficients {
    float PbAvg;
    float PrAvg;
    float a;
    float b;
    float c;
    float d;
};
/* quantize */
A2Methods_UArray2 quantize(A2Methods_UArray2 packedArray);
/* unquantize */
A2Methods_UArray2 unquantize(A2Methods_UArray2 quantArray);

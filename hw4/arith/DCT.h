/* DCT.h
 *
 * header file for the 3rd step of the compression/ decompression process. 
 *      convert y, pb, pr values to pb avg., pr avg., a, b, c, & d values 
 *      per 2x2 block 
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "quant.h"

// move to .c file 
struct components
{
    float y;
    float pb;
    float pr;
};
 
/* use discrete cosine transform to get coefficients */
A2Methods_UArray2 DiscreteCosineTransform(A2Methods_UArray2 array);
/* use inverse DCT to get back to component video color space */
A2Methods_UArray2 inverseDCT(A2Methods_UArray2 packedArray);
 
 
 
/* quant.c
 *
 * file for quantizing
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "quant.h"

/* function definitions */
void convert(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
void unconvert(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
int determineInterval(float num);
float unDetermineInterval(signed num);
float enforcePBounds(float val);

/* quantize 
 *
 *    Purpose: create a new UArray2 to quantize pb & pr and convert a, b, c, d
 *             to appropriate singed and unsigned ints 
 *
 * Parameters: UArray2 with pb, pr, a, b, c, d values per pixel
 *
 *    Returns: new UArray2 
*/
A2Methods_UArray2 quantize(A2Methods_UArray2 packedArray)
{
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(packedArray);
    int height = methods->height(packedArray);
    A2Methods_UArray2 quantArray = methods->new(width, height, 
                                   sizeof(struct bits));
    methods->map_row_major(quantArray, convert, packedArray);
    methods->free(&packedArray);
    return quantArray;
                                   
}

/* unquantize 
 *
 *    Purpose: Create the components of a ppm image by using the packed 
 *             componnents.
 *
 * Parameters: UArray2 with pb, pr, a, b, c, d values per pixel
 *
 *    Returns: new UArray2 
*/
A2Methods_UArray2 unquantize(A2Methods_UArray2 quantArray)
{
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(quantArray);
    int height = methods->height(quantArray);
    A2Methods_UArray2 packedArray = methods->new(width, height, 
                                   sizeof(struct coefficients));
    methods->map_row_major(packedArray, unconvert, quantArray);
    methods->free(&quantArray);
    return packedArray;
}

/* convert 
 *
 *    Purpose: performed mapped quantization
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void convert(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 packedArray = cl;
    struct coefficients *coeffs = methods->at(packedArray, i, j);
    struct bits bit;
    
    //check range 
    bit.PbChroma = Arith40_index_of_chroma(coeffs->PbAvg);
    bit.PrChroma = Arith40_index_of_chroma(coeffs->PrAvg);
    
    //check range 
    bit.a = (unsigned)round(coeffs->a * 63.0);
    bit.b = determineInterval(coeffs->b);
    bit.c = determineInterval(coeffs->c);
    bit.d = determineInterval(coeffs->d);
    
    *(struct bits*)elem = bit;
    (void)array;
    
}

int determineInterval(float num)
{
    if (num < -0.3) {
        num = -0.3;
    }
    if (num > 0.3) {
        num = 0.3;
    }
    
    float sint = num * (31.0 / 0.3);
    signed result  = round(sint);
    
    return result;
}

/* unconvert 
 *
 *    Purpose: performed mapped unquantization
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void unconvert(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 quantArray = cl;
    struct bits *bit = methods->at(quantArray, i, j);
    struct coefficients coeffs;
    
    //check range 
    coeffs.PbAvg = Arith40_chroma_of_index(bit->PbChroma);
    coeffs.PrAvg = Arith40_chroma_of_index(bit->PrChroma);
    
    coeffs.PbAvg = enforcePBounds(coeffs.PbAvg);
    coeffs.PrAvg = enforcePBounds(coeffs.PrAvg);
    
    //check range 
    coeffs.a = (float)bit->a / 63.0;
    coeffs.b = unDetermineInterval(bit->b);
    coeffs.c = unDetermineInterval(bit->c);
    coeffs.d = unDetermineInterval(bit->d);
    
    *(struct coefficients*)elem = coeffs;
    (void)array;
}

/* unDetermineInterval
 *
 *    Purpose: Check the bounds of the RGB values.
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
float unDetermineInterval(signed num)
{
    float result = (float)num / (31.0 / 0.3);
    
    if (result < -0.3) {
        result = -0.3;
    }
    if (result > 0.3) {
        result = 0.3;
    }
    return result;
}

/* enforcePBounds
 *
 *    Purpose: Check the bounds of the RGB values.
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
float enforcePBounds(float val)
{
    if (val < -0.5) {
        return -0.5;
    }
    
    if (val > 0.5) {
        return 0.5;
    }
    
    return val;
}
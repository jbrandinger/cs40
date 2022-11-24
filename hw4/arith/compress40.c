/* compress40.c
 *
 * Purpose: determine wether to copmpress or decompress the image 
 *
 * Created By: Joel Brandinger & Andrew Maynard
 *
 * Date: March 2022
 */
 
#include "RGBtypeConvert.h"

void trim(Pnm_ppm image);

/* compress40
 *
 *    Purpose: compress ppm to a compressed binary image 
 *
 * Parameters: input stream 
 *
 *    Returns: none
 *
 * Expectations: valid input stream
*/
extern void compress40(FILE *input)
{
    //read ppm file & trim
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    Pnm_ppm image = Pnm_ppmread(input, methods);
    assert(image->width >= 2);
    assert(image->height >= 2);
    trim(image);
    
    //compression
    A2Methods_UArray2 floatArray = convert2float(image); //step 1
    toComponent(floatArray); //step 2
    A2Methods_UArray2 packedArray = DiscreteCosineTransform(floatArray); //3
    A2Methods_UArray2 quantArray = quantize(packedArray); //step 4
    A2Methods_UArray2 wordArray = packBits(quantArray); //step 5
    printCompressedImage(wordArray); //step 6
    
}

/* decompress40
 *
 *    Purpose: decompress compressed binary image to a ppm 
 *
 * Parameters: input stream 
 *
 *    Returns: none
 *
 * Expectations: valid input stream
*/
extern void decompress40(FILE *input)
{
    //decompression
    A2Methods_UArray2 wordArray = readCompressedImage(input); // step 1
    A2Methods_UArray2 dcquantArray = unpackBits(wordArray); // step 2
    A2Methods_UArray2 dcPackedArray = unquantize(dcquantArray); // step 3
    A2Methods_UArray2 dcFloatArray = inverseDCT(dcPackedArray); //step 4
    toRGB(dcFloatArray); //step 5
    Pnm_ppm newImage = convert2int(dcFloatArray); // step 6
    //use ppm write to print decompressed image 
    Pnm_ppmwrite(stdout, newImage);
    Pnm_ppmfree(&newImage);
}

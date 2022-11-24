/* RGBtypeConvert.h
 *
 * header file for the first step of the compression/ decompression process 
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "colorspace.h"

/* convert ppm RGB values to floats */
A2Methods_UArray2 convert2float(Pnm_ppm image);
/* print the resulting array of convert2float */
void printArray(A2Methods_UArray2 array);
/* convert compressed file back to ppm */
Pnm_ppm convert2int(A2Methods_UArray2 floatArray);
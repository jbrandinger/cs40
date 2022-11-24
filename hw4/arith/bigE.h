/* bigE.h
 *
 * Header file for the final step in compression and the first step in
 * decompression.
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */

 #include <string.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <assert.h>
 #include <a2methods.h>
 #include <pnm.h>
 #include <arith40.h>
 #include <bitpack.h>
 #include "a2plain.h"
 #include "uarray2.h"
 #include <math.h> 
 
void printCompressedImage(A2Methods_UArray2 wordArray);
A2Methods_UArray2 readCompressedImage(FILE *fp);
/* colorspace.h
 *
 * header file for the 2nd step of the compression/ decompression process. 
 * That is, converting between RGB and component video color space.
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */

#include "DCT.h"

// move to .c file before submitting
struct vals {
    float red;
    float blue;
    float green;
};

/* convert to component color space */
void toComponent(A2Methods_UArray2 array);
/* convert to RGB color space */
void toRGB(A2Methods_UArray2 array);


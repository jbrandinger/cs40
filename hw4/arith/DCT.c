/* DCT.c
 *
 * compress component video colorspace to pb avg., pr avg., a, b, c, & d 
 *        values per 2x2 block 
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "DCT.h"

/* function definitions */
void transform(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
void untransform(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
float checkPBounds(float val);
struct components checkAllBounds(struct components comp);
void checkPointerBounds(struct components *comp);

/* DiscreteCosineTransform 
 *
 *    Purpose: create a new UArray2 1/4 of the size (each 2x2 block = 1 pixel)
 *             and convert y, pb, pr to pb avg., pr avg., a, b, c, & d
 *
 * Parameters: UArray2 with y, pb, & pr values per pixel
 *
 *    Returns: new UArray2 
*/
A2Methods_UArray2 DiscreteCosineTransform(A2Methods_UArray2 array)
{
    A2Methods_T methods = uarray2_methods_plain;
    int newWidth = methods->width(array) / 2;
    int newHeight = methods->height(array) / 2;
    A2Methods_UArray2 packedArray = methods->new(newWidth, newHeight, 
                                        sizeof(struct coefficients));
    methods->map_row_major(packedArray, transform, array);
    methods->free(&array);
    return packedArray;
}

/* transform 
 *
 *    Purpose: fill array with new desired chroma values and coefficients
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void transform(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct coefficients coeffs;
    A2Methods_UArray2 arr = cl;
    A2Methods_T methods = uarray2_methods_plain;
    
    i = i * 2;
    j = j * 2;
    
    struct components *sc1 = methods->at(arr, i, j);
    struct components *sc2 = methods->at(arr, i + 1, j);
    struct components *sc3 = methods->at(arr, i, j + 1);
    struct components *sc4 = methods->at(arr, i + 1, j + 1);
    
    checkPointerBounds(sc1);
    checkPointerBounds(sc2);
    checkPointerBounds(sc3);
    checkPointerBounds(sc4);
    
    coeffs.PbAvg = (sc1->pb + sc2->pb + sc3->pb + sc4->pb) / 4.0;
    coeffs.PrAvg = (sc1->pr + sc2->pr + sc3->pr + sc4->pr) / 4.0;
    coeffs.a = (sc4->y + sc3->y + sc2->y + sc1->y) / 4.0;
    coeffs.b = (sc4->y + sc3->y - sc2->y - sc1->y) / 4.0;
    coeffs.c = (sc4->y - sc3->y + sc2->y - sc1->y) / 4.0;
    coeffs.d = (sc4->y - sc3->y - sc2->y + sc1->y) / 4.0;
    
    coeffs.PbAvg = checkPBounds(coeffs.PbAvg);
    coeffs.PrAvg = checkPBounds(coeffs.PrAvg);
    
    
    *(struct coefficients*)elem = coeffs;
    (void)array;
}
/* inverseDCT
 *
 *    Purpose: convert back from compressed array to array of original size.
 *             convert pb avg., pr avg., a, b, c, & d to y, pb, pr
 *
 * Parameters: UArray2 with pb avg., pr avg., a, b, c, & d per pixel 
 *
 *    Returns: new UArray2 
*/
A2Methods_UArray2 inverseDCT(A2Methods_UArray2 packedArray)
{
    A2Methods_T methods = uarray2_methods_plain;
    int newWidth = methods->width(packedArray) * 2;
    int newHeight = methods->height(packedArray) * 2;
    A2Methods_UArray2 floatArray = methods->new(newWidth, newHeight, 
                                        sizeof(struct components));
    methods->map_row_major(packedArray, untransform, floatArray);
    methods->free(&packedArray);
    return floatArray;
}

/* untransform 
 *
 *    Purpose: convert back to component video color space from chroma 
 *             values and coefficients
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void untransform(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct components comps1;
    struct components comps2;
    struct components comps3;
    struct components comps4;
    
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 arr = cl;
    struct coefficients *val = elem;
    
    i = i * 2;
    j = j * 2;
    
    comps1.pb = val->PbAvg;
    comps2.pb = val->PbAvg;
    comps3.pb = val->PbAvg;
    comps4.pb = val->PbAvg;
    
    comps1.pr = val->PrAvg;
    comps2.pr = val->PrAvg;
    comps3.pr = val->PrAvg;
    comps4.pr = val->PrAvg;
    
    comps1.y = val->a - val->b - val->c + val->d;
    comps2.y = val->a - val->b + val->c - val->d;
    comps3.y = val->a + val->b - val->c - val->d;
    comps4.y = val->a + val->b + val->c + val->d;
    
    struct components finalcomps1 = checkAllBounds(comps1);
    struct components finalcomps2 = checkAllBounds(comps2);
    struct components finalcomps3 = checkAllBounds(comps3);
    struct components finalcomps4 = checkAllBounds(comps4);
    
    *(struct components*)methods->at(arr, i, j) = finalcomps1;
    *(struct components*)methods->at(arr, i + 1, j) = finalcomps2;
    *(struct components*)methods->at(arr, i, j + 1) = finalcomps3;
    *(struct components*)methods->at(arr, i + 1, j + 1) = finalcomps4;
    
    (void)array;
}

/* checkPBounds
 *
 *    Purpose: Check the bounds of the chroma
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
float checkPBounds(float val)
{
    if (val < -0.5) {
        return -0.5;
    }
    
    if (val > 0.5) {
        return 0.5;
    }
    
    return val;
}

/* checkAllBounds
 *
 *    Purpose: Check the bounds of the RGB values.
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
struct components checkAllBounds(struct components comp)
{
    if (comp.y < 0.0) {
        comp.y = 0.0;
    }
    
    if (comp.y > 1.0) {
        comp.y = 1.0;
    }
    
    comp.pb = checkPBounds(comp.pb);
    comp.pr = checkPBounds(comp.pr);
    
    return comp;
    
}

/* checkPointerBounds
 *
 *    Purpose: Check the bounds of the RGB values.
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
void checkPointerBounds(struct components *comp)
{
    // check y
    if (comp->y < 0.0) {
        comp->y = 0.0;
    }
    if (comp->y > 1.0) {
        comp->y = 1.0;
    }
    
    // check pb
    if (comp->pb < -0.5) {
        comp->pb = -0.5;
    }
    if (comp->pb > 0.5) {
        comp->pb = 0.5;
    }
    
    //check pr
    if (comp->pr < -0.5) {
        comp->pr = -0.5;
    }
    if (comp->pr > 0.5) {
        comp->pr = 0.5;
    }
    
}

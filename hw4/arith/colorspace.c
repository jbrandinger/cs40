/* colorspace.c
 *
 * compress RGB color space to component video color space or vice versa
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "colorspace.h"

/* function definitions */
void compute(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
void uncompute(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
float checkBounds(float val);
float checkYbound(float val);
float checkColor(float val);

/* toComponent 
 *
 *    Purpose: convert array structs to component video color space
 *             by computing y, pb, & pr from red, green, & blue.
 *
 * Parameters: UArray2 
 *
 *    Returns: none 
*/
void toComponent(A2Methods_UArray2 array)
{
    A2Methods_T methods = uarray2_methods_plain;
    methods->map_row_major(array, compute, NULL);
}

/* toRGB
 *
 *    Purpose: convert array structs to rgb color space
 *             by computing red, green, & blue from y, pb, & pr.
 *
 * Parameters: UArray2 
 *
 *    Returns: none 
*/
void toRGB(A2Methods_UArray2 array)
{
    A2Methods_T methods = uarray2_methods_plain;
    methods->map_row_major(array, uncompute, NULL);
}

/* compute 
 *
 *    Purpose: convert array structs to component video color space
 *             by computing y, pb, & pr from red, green, & blue.
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void compute(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct components comps;
    struct vals *val = elem;
    
    comps.y = 0.299 * val->red + 0.587 * val->green + 0.114 * val->blue;
    comps.pb = -0.168736 * val->red - 0.331264 * val->green + 0.5 * val->blue;
    comps.pr = 0.5 * val->red - 0.418688 * val->green - 0.081312 * val->blue;
    
    comps.y = checkYbound(comps.y);
    comps.pb = checkColor(comps.pb);
    comps.pr = checkColor(comps.pr);
    
    *(struct components*)elem = comps;
    (void)array;
    (void)i;
    (void)j;
    (void)cl;
}

/* uncompute 
 *
 *    Purpose: convert array structs to rgb color space
 *             by computing red, green, & blue from y, pb, & pr.
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: none 
*/
void uncompute(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct vals values;
    struct components *comps = elem;
    
    values.red = 1.0 * comps->y + 0.0 * comps->pb + 1.402 * comps->pr;
    values.green = 1.0 * comps->y - 0.344136 * comps->pb - 0.714136 
                       * comps->pr;
    values.blue = 1.0 * comps->y + 1.772 * comps->pb + 0.0 * comps->pr;
    
    // check pbar and prbar
    values.red = checkBounds(values.red);
    values.green = checkBounds(values.green);
    values.blue = checkBounds(values.blue);
    
    *(struct vals*)elem = values;
    (void)array;
    (void)i;
    (void)j;
    (void)cl;
}

/* checkBounds
 *
 *    Purpose: Check the bounds of the RGB values.
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
float checkBounds(float val)
{
    if (val < 0.0) {
        return 0.0;
    }
    
    if (val > 255.0) {
        return 255.0;
    }
    
    return val;
}

/* checkYBound
 *
 *    Purpose: Check the Y value of the component color space boundaires
 *
 * Parameters: The value of the RGB
 *
 *    Returns: The updated value if needed
*/
float checkYbound(float val)
{
    if (val < 0.0) {
        return 0.0;
    }
    
    if (val > 1.0) {
        return 1.0;
    }
    
    return val;
}

/* checkColor
 *
 *    Purpose: Check the componenet color space boundaries
 *
 * Parameters: The value
 *
 *    Returns: The updated value if needed
*/
float checkColor(float val)
{
    if (val < -0.5) {
        return -0.5;
    }
    
    if (val > 0.5) {
        return 0.5;
    }
    
    return val;
}
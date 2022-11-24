/* compress40.c
 *
 * Purpose: determine wether to copmpress or decompress the image 
 *
 * Created By: Joel Brandinger & Andrew Maynard
 *
 * Date: March 2022
 */
 
#include "RGBtypeConvert.h"

void fillArray(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
void printFloats(int i, int j, A2Methods_UArray2 array, void *elem, 
                   void *cl);
void printRGBs(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
void decompressArray(int i, int j, A2Methods_UArray2 array, void *elem, 
                     void *cl);
//void copy(int i, int j, A2Methods_UArray2 newPixels, void *elem, void *cl);
void printWords(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);
float checkFinalBounds(float val);

/* convert2float
 *
 *    Purpose: convert unsigned scaled integers from ppm image to floats 
 *             and store them in a new 2d array 
 *
 * Parameters: ppm image
 *
 *    Returns: UArray2 with structs of red, green, and blue float values 
 *
 * Expectations: valid ppm image 
*/
A2Methods_UArray2 convert2float(Pnm_ppm image)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 floatArray = methods->new(image->width, image->height, 
                                  sizeof(struct vals));                            
    methods->map_row_major(floatArray, fillArray, image);
    Pnm_ppmfree(&image);
    
    return floatArray;
}

/* convert2int
 *
 *    Purpose: convert floats from 2d array to unsigned scaled integers in 
 *             a ppm.
 *
 * Parameters: UArray2 of structs with floats and ppm image
 *
 *    Returns: ppm image in valid format of pnm.h
 *
 * Expectations: valid UArray2 
*/
Pnm_ppm convert2int(A2Methods_UArray2 floatArray)
{
    A2Methods_T methods = uarray2_methods_plain;
    
    int width = methods->width(floatArray);
    int height = methods->height(floatArray);
    
    A2Methods_UArray2 intArray = methods->new(width, height, 
                                              sizeof(struct Pnm_rgb));
    methods->map_row_major(intArray, decompressArray, floatArray);
    
    struct Pnm_ppm *pixmap = malloc(sizeof(struct Pnm_ppm));
    pixmap->width = width;
    pixmap->height = height;
    pixmap->denominator = 255;
    pixmap->pixels = intArray;
    pixmap->methods = methods;
              
    methods->free(&floatArray);
    
    return pixmap;
    
}

/* fillArray
 *
 *    Purpose: convert usigned scaled integers to floats and store them in 
 *             a new 2d array 
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
 *    Returns: UArray2 with structs of red, green, and blue float values 
 *
 * Expectations: valid ppm image 
*/
void fillArray(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct vals values;
    struct Pnm_rgb *originalPixels;
    Pnm_ppm image = cl;
    originalPixels = image->methods->at(image->pixels, i, j);
    
    values.red = ((float)originalPixels->red) / ((float)image->denominator);
    values.blue = ((float)originalPixels->blue) / ((float)image->denominator);
    values.green = ((float)originalPixels->green) / 
                   ((float)image->denominator);
    
    *(struct vals*)elem = values;
    (void)array;
}

/* printArray
 *
 *    Purpose: print red, green, and blue values of each struct stored 
 *             at each pixel to make sure they were properly converted
 *             to floats and scaled using the denominator. Implemented 
 *             as apply function for map row major.
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
 *
*/
void printArray(A2Methods_UArray2 array)
{
    A2Methods_T methods = uarray2_methods_plain;
    //methods->map_row_major(array, printFloats, NULL);
    //methods->map_row_major(array, printRGBs, NULL);
    methods->map_row_major(array, printWords, NULL);
}
/* printElements
 *
 *    Purpose: print red, green, and blue values of each struct stored 
 *             at each pixel to make sure they were properly converted
 *             to floats and scaled using the denominator. Implemented 
 *             as apply function for map row major.
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
*/
void printFloats(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct vals *val = elem;
    
    printf("%f %f %f\n", val->red, val->green, val->blue);
    (void) i;
    (void) j;
    (void) array;
    (void) cl;
}

/* printRGBs
 *
 *    Purpose: print red, green, and blue values of each struct stored 
 *             at each pixel to make sure they were properly converted
 *             to floats and scaled using the denominator. Implemented 
 *             as apply function for map row major.
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
*/
void printRGBs(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    struct Pnm_rgb *val = elem;
    
    printf("%u %u %u\n", val->red, val->green, val->blue);
    (void) i;
    (void) j;
    (void) array;
    (void) cl;
}

/* decompressArray
 *
 *    Purpose: decompress the array and update the ppm with the new RGB 
 *             values 
 *
 * Parameters: index i, j, UArray2, element at index, and closure 
*/
void decompressArray(int i, int j, A2Methods_UArray2 array, void *elem, 
                                                            void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 floatArray = cl;
    struct vals *pixel = methods->at(floatArray, i, j);
    struct Pnm_rgb val;
    
    float denominator = 255.0;
            
    val.red = checkFinalBounds(pixel->red * denominator);
    val.green = checkFinalBounds(pixel->green * denominator);
    val.blue = checkFinalBounds(pixel->blue * denominator);
    
    
    
    *(struct Pnm_rgb*)elem = val;
    
    (void)array;
}

/* printWords
 *
 *    Purpose: Print the fully compressed image to standard output.
 *
 * Parameters: index i, j, UArray2, element at index, and closure
 *
 *    Returns: None
*/
void printWords(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    uint64_t word = *(uint64_t*)elem;
    printf("%lu\n", word);
    
    
    (void) i;
    (void) j;
    (void) array;
    (void) cl;
}

/* trim
 *
 *    Purpose: check to see if width and height of the image are even. If
 *             either is odd, subtract 1 to make it even.
 *
 * Parameters: ppm image to be trimmed
 *
 *    Returns: none
 *
 * Expectations: valid ppm image 
*/
void trim(Pnm_ppm image)
{
    if (image->width % 2 == 1) {
        image->width = image->width - 1;
    }
    
    if (image->height % 2 == 1) {
        image->height = image->height - 1;
    }
}

/* trim
 *
 *    Purpose: Make sure RGB values are not too far out of range.
 *
 * Parameters: The value
 *
 *    Returns: The adjusted float
 *
 * Expectations: None
*/
float checkFinalBounds(float val)
{
    if (val < 0.0) {
        return 0.0;
    }
    
    if (val > 255.0) {
        return 255.0;
    }
    
    return val;
}
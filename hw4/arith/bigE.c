/* bigE.c
 *
 * File used to print the fully compressed file to standard output in 
 * big endian order as well as take in a P2 compressed file in big endian
 * order and convert it into an A2 methods array ready for further
 * decompression.
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "bigE.h"

/* function definitions */
void printBigE(int i, int j, A2Methods_UArray2 wordArray, void *elem, 
    void *cl);
void readBigE(int i, int j, A2Methods_UArray2 wordArray, void *elem, void *cl);

/* printCompressedImage
 *
 *    Purpose: Prints the header of the image and calls the mapping function 
 *             to print in big endian order.
 *
 * Parameters: The array with the packed pixels
 *
 *    Returns: None
*/
void printCompressedImage(A2Methods_UArray2 wordArray)
{
    A2Methods_T methods = uarray2_methods_plain;
    unsigned width = methods->width(wordArray) * 2;
    unsigned height = methods->height(wordArray) * 2;
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
    methods->map_row_major(wordArray, printBigE, NULL);
    methods->free(&wordArray);
}

/* printBigE
 *
 *    Purpose: Mapping function that prints the array of pixels out in big
 *             endian order.
 *
 * Parameters: index i, j, UArray2, element at index, and closure.
 *
 *    Returns: none 
*/
void printBigE(int i, int j, A2Methods_UArray2 wordArray, void *elem, void *cl)
{
    uint64_t word = *(uint64_t*)elem;
    
    for (int x = 24; x >= 0; x-=8) {
        int b = Bitpack_getu(word, 8, x);
        putchar(b);
    }
    (void)i;
    (void)j;
    (void)cl;
    (void)wordArray;
}

/* readCompressedImage
 *
 *    Purpose: Read in an image in compressed format 
 *
 * Parameters: index i, j, UArray2, element at index, and closure.
 *
 *    Returns: none 
*/
A2Methods_UArray2 readCompressedImage(FILE *fp)
{
    A2Methods_T methods = uarray2_methods_plain;
    unsigned height, width;
    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width, 
                      &height);
    assert(height >= 2);
    assert(width >= 2);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');

    A2Methods_UArray2 wordArray = methods->new(width / 2, height / 2, 
                                               sizeof(uint64_t));
    methods->map_row_major(wordArray, readBigE, fp);
    
    return wordArray;
}

/* buildWords
 *
 *    Purpose: Take the quantized array elements and replace them 
 *             with bit packed words.
 *
 * Parameters: index i, j, UArray2, element at index, and closure as the
 *             old quantized array.
 *
 *    Returns: none 
*/
void readBigE(int i, int j, A2Methods_UArray2 wordArray, void *elem, void *cl)
{
    uint64_t word = 0;
    FILE *fp = cl;
    
    for (int x = 24; x >= 0; x-=8) {
        int z = getc(fp);
        assert(z != EOF);
        word = Bitpack_newu(word, 8, x, z);
    }
    
    *(uint64_t*)elem = word;
    
    (void)i;
    (void)j;
    (void)wordArray;
}
/* pack.c
 *
 * pack all the values needed to represent one pixel into a 32 bit word
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */
 
#include "pack.h"

void buildWords(int i, int j, A2Methods_UArray2 wordArray, void *elem, 
    void *cl);
void unbuildWords(int i, int j, A2Methods_UArray2 wordArray, void *elem, 
    void *cl);

/* packBits
 *
 *    Purpose: Take the array of pixels that has been quantized and pack
 *             all the information needed for a pixel into one 32 bit word.
 *
 * Parameters: The quantized array to be packed
 *
 *    Returns: The newly packed array.
*/
A2Methods_UArray2 packBits(A2Methods_UArray2 quantArray)
{
    // create new a2
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(quantArray);
    int height = methods->height(quantArray);
    A2Methods_UArray2 wordArray = methods->new(width, height, 
                                   sizeof(uint64_t));
    methods->map_row_major(wordArray, buildWords, quantArray);
    methods->free(&quantArray);
    return wordArray;
}

/* unpackBits
 *
 *    Purpose: Take a packed array and unpack the components and place
 *             each part of the 32 bit word into their respective stuct
 *             components.
 *
 * Parameters: The array with all pixels represented as packed array.
 *
 *    Returns: The unpacked and quantized array.
*/
A2Methods_UArray2 unpackBits(A2Methods_UArray2 wordArray)
{
    // create new a2
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(wordArray);
    int height = methods->height(wordArray);
    A2Methods_UArray2 quantArray = methods->new(width, height, 
                                   sizeof(struct bits));
    methods->map_row_major(quantArray, unbuildWords, wordArray);
    methods->free(&wordArray);
    return quantArray;
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
void buildWords(int i, int j, A2Methods_UArray2 wordArray, void *elem, 
                void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 quantArray = cl;
    struct bits *bit = methods->at(quantArray, i, j);
    
    uint64_t word = 0;
    word = Bitpack_newu(word, 4, 0, bit->PrChroma);
    word = Bitpack_newu(word, 4, 4, bit->PbChroma);
    word = Bitpack_news(word, 6, 8, bit->d);
    word = Bitpack_news(word, 6, 14, bit->c);
    word = Bitpack_news(word, 6, 20, bit->b);
    word = Bitpack_newu(word, 6, 26, bit->a);
    
    *(uint64_t*)elem = word;
    (void)wordArray;
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
void unbuildWords(int i, int j, A2Methods_UArray2 quantArray, void *elem, 
                  void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 wordArray = cl;
    
    uint64_t *word = methods->at(wordArray, i, j);
    struct bits bit;
    
    bit.PrChroma = Bitpack_getu(*word, 4, 0);
    bit.PbChroma = Bitpack_getu(*word, 4, 4);
    bit.d = Bitpack_gets(*word, 6, 8);
    bit.c = Bitpack_gets(*word, 6, 14);
    bit.b = Bitpack_gets(*word, 6, 20);
    bit.a = Bitpack_getu(*word, 6, 26);
    
    *(struct bits*)elem = bit;
    (void)quantArray;
}
 
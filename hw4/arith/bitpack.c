/* bitpack.c
 *
 * Pack different sizes of information into a 64bit word.
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */

#include "bitpack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "except.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* function definitions */
uint64_t twoRaised(uint64_t num);

/* Bitpack_fitsu
 *
 *    Purpose: check to see if unsigned int fits in width bits 
 *
 * Parameters: unsigned number to check and "width" the numer of bits 
 *
 *    Returns: true if it fits, false if not
 */ 
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    if (width == 64) {
        return true;
    }
    
    if (n < twoRaised(width)) {
        return true;
    } else {
        return false;
    }
}

/* twoRaised
 *
 *    Purpose: Raise any number to the power of two.
 *
 * Parameters: The number of times to raise a number to the power of two.
 *
 *    Returns: The final product
 */
uint64_t twoRaised(uint64_t num)
{
    return (uint64_t) 1 << num;
}

/* Bitpack_fitss
 *
 *    Purpose: check to see if signed int fits in width bits 
 *
 * Parameters: signed number to check and "width" the numer of bits 
 *
 *    Returns: true if it fits, false if not
 */ 
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);
    
    if (width == 64) {
        if (n > (int64_t)(twoRaised(32) - 1) || 
            n < (int64_t)(twoRaised(32)* -1)) {
            return false;
        } else {
            return true;
        }
    }
    
    int64_t max = (twoRaised(width) / 2) - 1;
    int64_t min = (twoRaised(width) / 2) * -1;
    
    if (n > max || n < min) {
        return false;
    } else {
        return true;
    }
}

/* Bitpack_getu
 *
 *    Purpose: extract unsigned value from word given least significant bit 
 *             and width
 *
 * Parameters: word, width/ number of bits, & least significant bit
 *
 *    Returns: unsigned value stored in that section of the word 
 */ 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64); 
    assert((lsb + width) <= 64);
    
    uint64_t mask = ~0;
    mask = mask >> (64 - (uint64_t)width) << (uint64_t)lsb;
    
    uint64_t result = word & mask;
    result = result >> (uint64_t)lsb;
    
    return result;
}

/* Bitpack_gets
 *
 *    Purpose: extract signed value from word given least significant bit 
 *             and width
 *
 * Parameters: word, width/ number of bits, & least significant bit
 *
 *    Returns: signed value stored in that section of the word 
 */ 
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64); 
    assert((lsb + width) <= 64);
    
    uint64_t mask = ~0;
    mask = mask >> (64 - (uint64_t)width) << (uint64_t)lsb;
    
    int64_t result = word & mask;
    result = result << (64 - (uint64_t)width - (uint64_t)lsb);
    
    result = result >> (64 - (uint64_t)width);
    
    return result;
}

/* Bitpack_newu
 *
 *    Purpose: return a new word that has been updated in the desired field
 *             with the desired unsigned value
 *
 * Parameters: word, width/ number of bits, least significant bit, & new 
 *             unsigned value to be stored in a certain field of the word
 *
 *    Returns: new word 
 */ 
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
    assert(width <= 64); 
    assert((lsb + width) <= 64);
    
    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    
    value = value << (uint64_t)lsb;
    
    uint64_t mask = ~0;
    mask = mask >> (64 - (uint64_t)width);
    mask = mask << (uint64_t)lsb;
    mask = ~mask;
    
    uint64_t newWord = word & mask;
    newWord = newWord | value;
    
    return newWord;
}

/* Bitpack_news
 *
 *    Purpose: return a new word that has been updated in the desired field
 *             with the desired signed value
 *
 * Parameters: word, width/ number of bits, least significant bit, & new 
 *             signed value to be stored in a certain field of the word
 *
 *    Returns: new word 
 */ 
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  
                      int64_t value)
{
    assert(width <= 64); 
    assert((lsb + width) <= 64);
    
    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    
    uint64_t x = value << (64 - (uint64_t)width);
    x = x >> (64 - (uint64_t)width);
    x = x << (uint64_t)lsb;
    
    uint64_t mask = ~0;
    mask = mask >> (64 - (uint64_t)width);
    mask = mask << (uint64_t)lsb;
    mask = ~mask;
    
    uint64_t newWord = word & mask;
    newWord = newWord | x;
    
    return newWord;
}
 
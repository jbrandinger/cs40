/* bitpacktests.c
 *
 * test the functionalty of bitpack
 *
 * Created By: Joel Brandinger & Andrew Maynard 
 * Date: March 2022 
 */

#include "bitpack.h"
#include <stdlib.h>
#include <stdio.h>
 
int main()
{   
    // // test Bitpack_fitsu 
    // printf("Testing fit for unsigned\n");
    // if (Bitpack_fitsu(7, 3)) {
    //     printf("TRUE!\n");
    // } else {
    //     printf("FALSE!\n");
    // }
    // 
    // // test Bitpack_fitss
    // printf("Testing fit for signed\n");
    // if (Bitpack_fitss(-4, 3)) {
    //     printf("TRUE!\n");
    // } else {
    //     printf("FALSE!\n");
    // }
    // 
    // //prints 255,555,555 in binary form
    // printf("Code: 1111 0011 1011 0111 0111 1110 0011\n");
    // 
    // // 1 itest Bitpack_getu
    // printf("Test get unsigned\n");
    // uint64_t word1 = 255555555;
    // unsigned lsb1 = 0;
    // unsigned width1 = 28;
    // printf("Epected: 255555555 in binary ");
    // printf("Got: %lu\n", Bitpack_getu(word1, width1, lsb1));
    // 
    // // test Bitpack_gets
    // printf("Test get signed\n");
    // printf("Code: 1111 0011 1011 0111 0111 1110 0011\n");
    // uint64_t word2 = ~0;
    // word2 = word2 << 1;
    // unsigned lsb2 = 0;
    // unsigned width2 = 64;
    // printf("Expected: -2 ");
    // printf("Got: %ld\n", Bitpack_gets(word2, width2, lsb2));
    // 
    // // test new
    // uint64_t wordA = Bitpack_news(0, 4, 0, 0);
    // uint64_t wordB = Bitpack_news(wordA, 8, 8, 10);
    // 
    // printf("WORD: %lu\n", wordB);
    uint64_t word = 0;
    word = Bitpack_news(word, 5, 18, 1);
    int64_t x = Bitpack_gets(word, 5, 18);
    printf("NUmber = %ld\n", x);
    
    word = 0;
    word = Bitpack_newu(word, 5, 18, 3);
    x = Bitpack_getu(word, 5, 18);
    printf("NUmber = %ld\n", x);
    
    
    
    return 0;
}
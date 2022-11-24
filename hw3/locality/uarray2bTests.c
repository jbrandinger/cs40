/* 
 *   uarray2bTests.c
 *   by: Drew Maynard and Joel Brandinger, 02/21/22
 *   Locality
 *
 *   A unit testing file to make sure the blocked 2D array
 *   is correctly implemented.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "uarray2b.h"
#include "uarray2.h"
#include "uarray.h"

void
check_and_print(int i, int j, UArray2b_T a, void *p1, void *p2);

int main(int argc, char **argv)
{
    UArray2b_T blockedArray = UArray2b_new(101, 180, sizeof(char), 74);

    for (int i = 0; i < 180; i++) {
        for (int j = 0; j < 101; j++) {
            
            *(char*)UArray2b_at(blockedArray, j, i) = 'a';
        }
    }
    bool OK = true;
    UArray2b_map(blockedArray, check_and_print, &OK);
    UArray2b_free(&blockedArray);

    (void)argc;
    (void)argv;
    return 0;
}

void
check_and_print(int i, int j, UArray2b_T a, void *p1, void *p2) 
{
        char *entry_p = p1;

        *((bool *)p2) &= UArray2b_at(a, i, j) == entry_p;

        // if ( (i == (9 - 1) ) && (j == (9 - 1) ) ) {
        //          we got the corner 
        //         *((bool *)p2) &= (*entry_p == 99);
        // }

        printf("ar[%d,%d] %c \n", i, j, *(char*)UArray2b_at(a, i, j));
}
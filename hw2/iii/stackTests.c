/* stackTests.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file tests the functionality of the stack class.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

void printStack(Stack_T stack);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Stack_T test_stack;

    test_stack = Stack_new();
    void *junk;

    char *a = (char*)malloc(sizeof(char));
    *a = 'a';
    char *b = (char*)malloc(sizeof(char));
    *b = 'b';
    char *c = (char*)malloc(sizeof(char));
    *c = 'c';
    Stack_push(test_stack, a);
    Stack_push(test_stack, b);
    Stack_push(test_stack, c);
    junk = Stack_pop(test_stack);
    free(junk);
    junk = Stack_pop(test_stack);
    free(junk);
    junk = Stack_pop(test_stack);
    free(junk);
    Stack_free(&test_stack);
    return EXIT_SUCCESS;
}

void printStack(Stack_T stack)
{
    int size = Stack_size(stack);

    while (size > 0) {
        printf("%c \n", *(char*)Stack_pop(stack));
        size--;
    }
}
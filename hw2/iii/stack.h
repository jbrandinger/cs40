/* stack.h
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the interface of the stack class.
 *
*/

#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#include <seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define T Stack_T
typedef struct T *T;

extern T Stack_new();
extern void Stack_free(T *dataStack);

extern bool Stack_isEmpty(T dataStack);
extern int Stack_size(T dataStack);

extern void Stack_push(T dataStack, void *elmt);
extern void *Stack_pop(T dataStack);

#undef T
#endif


/* stack.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This file contains the implementation for the stack data structure.
 *
 * NOTE: Sequence is used for a stack and is in a struct to add an extra
 *       level of abstraction.
 *
*/

#include "stack.h"

#define T Stack_T

struct T {
    Seq_T stack;
};

/* Stack_new
 *
 *      Purpose: Create a new stack ready for use
 *   Parameters: None
 *      Returns: A pointer to a newly created stack
 *
 * Expectations: None
 *
*/
extern T Stack_new()
{
    T newStack = (void*)malloc(sizeof(struct T));
    assert(newStack != NULL);
    newStack->stack = Seq_new(0);
    return newStack;
}

/* Stack_free
 *
 *      Purpose: Free the allocated memory used to hold the sequence
 *   Parameters: A reference to the sequence used to hold the stack
 *      Returns: None
 *
 * Expectations: dataStack is not NULL
 *
*/
extern void Stack_free(T *dataStack)
{
    assert(dataStack);
    for (int i = 0; i < Seq_length((*dataStack)->stack); i++) {
        free(Seq_get((*dataStack)->stack, i));
    }

    Seq_free(&(*dataStack)->stack);
    free((*dataStack));
}

/* Stack_isEmpty
 *
 *      Purpose: Check to see if the stack is empty
 *   Parameters: The stack to check
 *      Returns: True if the stack is empty
 *
 * Expectations: None
 *
*/
extern bool Stack_isEmpty(T dataStack)
{
    assert(dataStack);
    return Seq_length(dataStack->stack) == 0;
}

/* Stack_size
 *
 *      Purpose: Get the number of element currently in the stack
 *   Parameters: The stack instance
 *      Returns: The number of elements
 *
 * Expectations: The element being added is not NULL
 *
*/
extern int Stack_size(T dataStack)
{
    assert(dataStack);
    return Seq_length(dataStack->stack);
}

/* Stack_push
 *
 *      Purpose: Push an element onto the stack
 *   Parameters: The stack and element to be pushed onto the list
 *      Returns: None
 *
 * Expectations: The element being added is not NULL
 *
*/
extern void Stack_push(T dataStack, void *elmt)
{
    assert(dataStack);
    assert(elmt != NULL);
    Seq_addhi(dataStack->stack, elmt);
}

/* Stack_pop
 *
 *      Purpose: Pop the top element off the stack
 *   Parameters: The stack instance
 *      Returns: A pointer to the element
 *
 * Expectations: The element being added is not NULL
 *
*/
extern void *Stack_pop(T dataStack)
{
    assert(dataStack);
    assert(Stack_size(dataStack) > 0);
    void *element = NULL;
    element = Seq_remhi(dataStack->stack);
    return element;
}




















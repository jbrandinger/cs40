/* fixEdge.h
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * File contains the interface for the fixEdge program.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bit2.h"
#include <pnmrdr.h>

#define Node Node
typedef struct Node *Node;

void fixEdge(FILE *inputfp);

#undef Node






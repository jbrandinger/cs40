/* unblackedges.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/09/22
 * Interfaces, Implementations, and Images (iii)
 *
 * This program takes in an image in pbm format and removes the black
 * edges.
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "performOperation.h"
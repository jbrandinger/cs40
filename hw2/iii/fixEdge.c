/* fixEdge.c
 *
 * By: Drew Maynard and Joel Brandinger, 02/08/22
 * Interfaces, Implementations, and Images (iii)
 *
 * File contains the implementation for the fixEdge program. These
 * functions will use the bit2 class to manipulate the data from a
 * supplied pbm P1 image. It will allow user to remove the black
 * edges in the image.
 *
*/

#include "fixEdge.h"
#include "stack.h"

void placeBit(int i, int j, Bit2_T bitmap, int bit, void *cl);
Bit2_T pbmread(FILE *inputfp);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
void removeEdges(Bit2_T bitmap);
void dfsRemove(Bit2_T bitmap, int col, int row);
void getNeighbors(Stack_T stack, Bit2_T bitmap, int col, int row);
bool inBounds(int maxRow, int maxCol, int row, int col);
Node newNode(Bit2_T bitmap, int col, int row);
void print(int i, int j, Bit2_T a, int b, void *p1); 

#define Node Node

struct Node {
    int x;
    int y;
    int bit;
};

/* fixEdge
 *
 *      Purpose: Runs all operations needed to fix the edges on the image.
 *
 *   Parameters: The input file stream (Assumed to be a pbm)
 *
 *      Returns: None.
 *
 * Expectations: None.
*/
void fixEdge(FILE *inputfp)
{
    Bit2_T bitmap = pbmread(inputfp);
    assert(bitmap);
    removeEdges(bitmap);
    pbmwrite(stdout, bitmap);
    Bit2_free(&bitmap);
}

/* pbmread
 *
 *      Purpose: Read in the pbm image and store the data in a 2D bit vector
 *
 *   Parameters: The input file stream (Assumed to be a pbm)
 *
 *      Returns: The bit2 vector
 *
 * Expectations: None 
*/
Bit2_T pbmread(FILE *inputfp)
{
    Pnmrdr_T image = Pnmrdr_new(inputfp);
    Pnmrdr_mapdata data = Pnmrdr_data(image);
    assert(data.type == 1);
    Bit2_T bitmap = Bit2_new(data.width, data.height);
    Bit2_map_row_major(bitmap, placeBit, image);
    Pnmrdr_free(&image);
    
    return bitmap;
}

/* placeBit
 *
 *      Purpose: Function is made to be used as the apply functions when
 *               mapping the bitmap in row major order. Places a bit in the
 *               correct location.
 *
 *   Parameters: The column, row, bitmap instance, the bit to place, and the
 *               instance of the Pnmrdr reader that reads the bits from input.
 *
 *      Returns: None.
 *
 * Expectations: cl is a pnmrdr instance and bitmap is Bit2.
*/
void placeBit(int i, int j, Bit2_T bitmap, int bit, void *cl)
{
    Bit2_put(bitmap, i, j, Pnmrdr_get(cl));
    (void)bit;
}

/* removeEdges
 *
 *      Purpose: Iterate along all the edges, if a bit is marked as black on
 *               the outside, then mark as white and run a dfs search on that
 *               outside black bit.
 *
 *   Parameters: The instance of the bitmap
 *
 *      Returns: None
 *
 * Expectations: None
*/
void removeEdges(Bit2_T bitmap)
{
    /* top row */
    for (int i = 0; i < Bit2_width(bitmap); i++) {
        if (Bit2_get(bitmap, i, 0) == 1) {
            dfsRemove(bitmap, i, 0);
        }
    }

    /* right column */
    for (int i = 0; i < Bit2_height(bitmap); i++) {
        if (Bit2_get(bitmap, Bit2_width(bitmap) - 1, i) == 1) {
            dfsRemove(bitmap, Bit2_width(bitmap) - 1, i);
        }
    }

    /* bottom row */
    for (int i = Bit2_width(bitmap) - 1; i >= 0; i--) {
        if (Bit2_get(bitmap, i, Bit2_height(bitmap) - 1) == 1) {
            dfsRemove(bitmap, i, Bit2_height(bitmap) - 1);
        }
    }

    /* left side */
    for (int i = Bit2_height(bitmap) - 1; i >= 0; i--) {
        if (Bit2_get(bitmap, 0, i) == 1) {
            dfsRemove(bitmap, 0, i);
        }
    }
}

/* dfsRemove
 *
 *      Purpose: Start at a position in a bitmap and remove all black 
 *               bits that are connected to the starting node.
 *
 *   Parameters: The bitmap, the column, and the row
 *
 *      Returns: None
 *
 * Expectations: Checks to make sure bitmap is updated correctly.
*/
void dfsRemove(Bit2_T bitmap, int col, int row)
{
    Node element = newNode(bitmap, col, row);
    Stack_T stack = Stack_new();
    Stack_push(stack, element);

    while (!Stack_isEmpty(stack)) {
        element = Stack_pop(stack);
        for (int i = 0; i < 4; i++) {
            getNeighbors(stack, bitmap, element->x, element->y);
        }
        Bit2_put(bitmap, element->x, element->y, 0);
        free(element);
    }

    Stack_free(&stack);
}

/* getNeighbors
 *
 *      Purpose: Gets all valid adjacent elements and adds them onto the
 *               stack if they are marked as black. Makes sure neighbor is
 *               in bounds and is black.
 *
 *   Parameters: The stack, bitmap, origonal column and row 
 *               index.
 *
 *      Returns: None
 *
 * Expectations: None
*/
void getNeighbors(Stack_T stack, Bit2_T bitmap, int col, int row)
{
    int maxCol = Bit2_width(bitmap) - 1;
    int maxRow = Bit2_height(bitmap) - 1;

    if (inBounds(maxCol, maxRow, col, row - 1)) {
        if (Bit2_get(bitmap, col, row - 1) == 1) {
            Node element = newNode(bitmap, col, row - 1);
            Stack_push(stack, element);
        }
    }

    if (inBounds(maxCol, maxRow, col + 1, row)) {
        if (Bit2_get(bitmap, col + 1, row) == 1) {
            Node element = newNode(bitmap, col + 1, row);
            Stack_push(stack, element);
        }
    }

    if (inBounds(maxCol, maxRow, col, row + 1)) {
        if (Bit2_get(bitmap, col, row + 1) == 1) {
            Node element = newNode(bitmap, col, row + 1);
            Stack_push(stack, element);
        }
    }

    if (inBounds(maxCol, maxRow, col - 1, row)) {
        if (Bit2_get(bitmap, col - 1, row) == 1) {
            Node element = newNode(bitmap, col - 1, row);
            Stack_push(stack, element);
        }
    }
}

/* inBounds
 *
 *      Purpose: Makes sure a row and column index is in bounds.
 *
 *   Parameters: The maximum column and row index.
 *
 *      Returns: True if it is in bounds.
 *
 * Expectations: column and row indices are valid.
*/
bool inBounds(int maxRow, int maxCol, int row, int col)
{
    if (row <= maxRow && row >= 0 && col <= maxCol && col >= 0) {
        return true;
    }

    return false;
}

/* newNode
 *
 *      Purpose: Create a new node on the heap so that an object can be
 *               placed onto the stack that represents a specific element
 *               in the bitmap.
 *
 *   Parameters: The bitmap, the column, and the row.
 *
 *      Returns: A pointer to the node.
 *
 * Expectations: The memory is allocated and the position in bitmap is black
*/
Node newNode(Bit2_T bitmap, int col, int row)
{
    Node nodep = (void*)malloc(sizeof(struct Node));
    assert(nodep != NULL);
    assert(Bit2_get(bitmap, col, row) == 1);

    nodep->x = col;
    nodep->y = row;
    nodep->bit = 1;
    return nodep;
}

/* pbmwrite
 *
 *      Purpose: Prints the revised image out in P1 format.
 *
 *   Parameters: The output file stream and the bitmap.
 *
 *      Returns: None
 *
 * Expectations: File stream is not null, bitmap is not null
*/
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
    assert(outputfp != NULL);
    assert(bitmap != NULL);

    fprintf(outputfp, "%s\n", "P1");
    fprintf(outputfp, "%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));

    Bit2_map_row_major(bitmap, print, outputfp);
}

/* print
 *
 *      Purpose: Prints the bit of current index.
 *
 *   Parameters: The column, the row, the bitmat, the bit at that position,
 *               and the closure argument.
 *
 *      Returns: None
 *
 * Expectations: None
*/
void print(int i, int j, Bit2_T bitmap, int b, void *p1) 
{
    assert(p1 != NULL);
    fprintf(p1, "%d", b);
    
    if (i == Bit2_width(bitmap) - 1) {
        fprintf(p1, "\n");
    }

    (void)j;
}























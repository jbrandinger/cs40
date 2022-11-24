#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

struct Closure {
        A2Methods_UArray2 rotatedImage;
        A2Methods_T methods;
};

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

FILE *openFile(char *filename, char *program);
FILE *openFileWrite(char *filename, char *program);
void determineRotation(Pnm_ppm image, int rotation, 
        A2Methods_mapfun *map, struct Closure *cl);
void rotate90(int i, int j, A2Methods_UArray2 pixels, void *val, void *cl);
void rotate180(int i, int j, A2Methods_UArray2 pixels, void *val, void *cl);
void printError();

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        int   i;

        CPUTime_T timer;
        double time_used;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        FILE* fp;

        if (argc - 1 == i) {
                fp = openFile(argv[i], argv[0]);
        } else {
                char filename[1000];
                scanf("%s", filename);
                fp = openFile(filename, argv[0]);
        }

        Pnm_ppm image = Pnm_ppmread(fp, methods);
        struct Closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;

        if (time_file_name != NULL) {
                FILE *fpT = openFileWrite(time_file_name, argv[0]);
                timer = CPUTime_New();
                CPUTime_Start(timer);
                determineRotation(image, rotation, map, cl);
                time_used = CPUTime_Stop(timer);
                double size = image->width * image->height;
                fprintf(fpT, "Size = %lf pixels\n", size);
                fprintf(fpT, "Total time = %lf nanoseconds\n", time_used);
                double tpp = time_used / size;
                double numSecondsPerPixel = time_used / 1000000000;
                double numInstructions = numSecondsPerPixel * 1000000000;
                fprintf(fpT, "Instructions per pixel = %lf instructions\n",
                 numInstructions);
                fprintf(fpT, "Time per pixel = %lf nanoseconds\n", tpp);
                CPUTime_Free(&timer);
                fclose(fpT);     
        } else {
                determineRotation(image, rotation, map, cl);
        }

        if (rotation != 0) {
                methods->free(&(image->pixels));
                image->pixels = cl->rotatedImage;
        }

        Pnm_ppmwrite(stdout, image);

        fclose(fp);
        Pnm_ppmfree(&image);
        free(cl);
        (void)  time_used;

        return 0;
}

/* openFile
 *
 *    Purpose: Open a file for the user and check for success
 *
 * Parameters: The name of the file and the name of the program
 *
 *    Returns: A pointer to the file stream that was opened
 *
*/
FILE *openFile(char *filename, char *program)
{
        FILE *fp = fopen(filename, "rb");

        if (fp == NULL) {
                fprintf(stderr, "%s: %s %s %s\n",
                        program, "Could not open file",
                        filename, "for reading.");
                exit(EXIT_FAILURE);
        } else {
                return fp;
        }
}

/* openFile
 *
 *    Purpose: Open a file for the user and check for success
 *
 * Parameters: The name of the file and the name of the program
 *
 *    Returns: A pointer to the file stream that was opened
 *
*/
FILE *openFileWrite(char *filename, char *program)
{
        FILE *fp = fopen(filename, "wb");

        if (fp == NULL) {
                fprintf(stderr, "%s: %s %s %s\n",
                        program, "Could not open file",
                        filename, "for reading.");
                exit(EXIT_FAILURE);
        } else {
                return fp;
        }
}

/* determineRotation
 *
 *    Purpose: Determines what rotation function to call for apply function.
 *
 * Parameters: The instance of A2Methods_UArray2, the rotation to use, and
 *             the mapping method to be used.
 *
 *    Returns: None.
 *
 * Exceptions: methods is not null.
*/
void determineRotation(Pnm_ppm image, int rotation, 
        A2Methods_mapfun *map, struct Closure *cl)
{
        assert(image);
        void *closure = cl;

        if (rotation == 0) {
                return;
        } else if (rotation == 90) {
                A2Methods_UArray2 rotatedImage = 
                        cl->methods->new(image->height,
                        image->width, sizeof(struct Pnm_rgb));
                cl->rotatedImage = rotatedImage;
                map(image->pixels, rotate90, closure);
                unsigned temp = image->height;
                image->height = image->width;
                image->width = temp;
        } else if (rotation == 180) {
                A2Methods_UArray2 rotatedImage = cl->methods->new(image->width,
                        image->height, sizeof(struct Pnm_rgb));
                cl->rotatedImage = rotatedImage;
                map(image->pixels, rotate180, closure);
        } else {
                printError();
        }
}

/* rotate90
 *
 *    Purpose: Rotate the image 90 degrees. This function is passed into
 *             a mapping function to transfer data from one A2 to another.
 *
 * Parameters: The integers from origional image, the A2 from origional
 *             image, the value at given indices, and an instance of a
 *             Closure struct that contains updated image methods and
 *             its A2.
 *
 *    Returns: Program exit code.
 *
 * Exceptions: None.
*/
void rotate90(int i, int j, A2Methods_UArray2 pixels, void *val, void *cl)
{
        struct Closure *closure = cl;
        
        int height = closure->methods->height(pixels);
        int newi = height - j - 1;
        int newj = i;

        struct Pnm_rgb *newVal = closure->methods->at(closure->rotatedImage, 
                newi, newj);
        struct Pnm_rgb *oldVal  = closure->methods->at(pixels,i, j);
        *newVal = *oldVal;

        (void)val;
}

/* rotate180
 *
 *    Purpose: Rotate the image 180 degrees. This function is passed into
 *             a mapping function to transfer data from one A2 to another.
 *
 * Parameters: The integers from origional image, the A2 from origional
 *             image, the value at given indices, and an instance of a
 *             Closure struct that contains updated image methods and
 *             its A2.
 *
 *    Returns: Program exit code.
 *
 * Exceptions: None.
*/
void rotate180(int i, int j, A2Methods_UArray2 pixels, void *val, void *cl)
{
        struct Closure *closure = cl;
        
        int height = closure->methods->height(pixels);
        int width = closure->methods->width(pixels);
        int newi = width - i - 1;
        int newj = height - j - 1;
        struct Pnm_rgb *newVal = closure->methods->at(closure->rotatedImage, 
                newi, newj);
        struct Pnm_rgb *oldVal  = closure->methods->at(pixels,i, j);
        *newVal = *oldVal;

        (void)val;
}

/* printError
 *
 *    Purpose: Print the an error code if the input operation is not
 *             implemented.
 *
 * Parameters: None.
 *
 *    Returns: None.
 *
 * Exceptions: None.
*/
void printError()
{
        fprintf(stderr, "Not implemented yet\n");
        exit(1);
}















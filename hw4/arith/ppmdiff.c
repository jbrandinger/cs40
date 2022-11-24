/* ppmdiff.c 
 * Purpose: diff two ppnm image files 
 * Created By: Joel Brandinger & Andrew Maynard
 * Date: February 25, 2022
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <pnm.h>
 #include <math.h>
 #include <a2methods.h>
 #include "a2plain.h"
 
 FILE *openFile(char *filename, char *program);
 double diff(Pnm_ppm image1, Pnm_ppm image2);
 double square (double x);
 void check_bounds(Pnm_ppm image1, Pnm_ppm image2);
 unsigned chooseHeight(Pnm_ppm image1, Pnm_ppm image2);
 unsigned chooseWidth(Pnm_ppm image1, Pnm_ppm image2);
 
 int main(int argc, char *argv[])
 {
     if (argc != 3) {
         fprintf(stderr, "Error! Usage: %s file1 file2", argv[0]);
     }
     FILE *fp1 = openFile(argv[1], "rb");
     FILE *fp2 = openFile(argv[2], "rb");
     
     A2Methods_T methods = uarray2_methods_plain;
     
     Pnm_ppm image1 = Pnm_ppmread(fp1, methods);
     Pnm_ppm image2 = Pnm_ppmread(fp2, methods);
     check_bounds(image1, image2);
     
     double result = diff(image1, image2);
     printf("result = %lf\n", result);
     
     Pnm_ppmfree(&image1);
     Pnm_ppmfree(&image2);
     
     fclose(fp1);
     fclose(fp2);
     
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
 
 /* diff
  *
  *    Purpose: perform the diff calculation on the two images 
  *
  * Parameters: two instances of a ppm image
  *
  *    Returns: the root mean square difference of the two images 
  *
 */
 double diff(Pnm_ppm image1, Pnm_ppm image2)
 {
     unsigned height = chooseHeight(image1, image2);
     unsigned width = chooseWidth(image1, image2);
     
     struct Pnm_rgb *val1, *val2;
     double red1, red2, green1, green2, blue1, blue2;
     double result = 0;
     for (unsigned j = 0; j < height; j++) {
         for (unsigned i = 0; i < width; i++) {
             val1 = image1->methods->at(image1->pixels, i, j);
             val2 = image2->methods->at(image2->pixels, i, j);
             red1 = (double)val1->red / (double)image1->denominator;
             red2 = (double)val2->red / (double)image2->denominator;
             green1 = (double)val1->green / (double)image1->denominator;
             green2 = (double)val2->green / (double)image2->denominator;
             blue1 = (double)val1->blue / (double)image1->denominator;
             blue2 = (double)val2->blue / (double)image2->denominator;
             
             result += square(red1 - red2) + 
                       square(green1 - green2) +
                       square(blue1 - blue2);
         }     
     }
     result = result / (3 * width * height);
     return sqrt(result);
 }
 
 /* square
  *
  *    Purpose: square a number
  *
  * Parameters: a double to be squared
  *
  *    Returns: the number squared
  *
 */
 double square (double x)
 {
     return x * x;
 }
 
 /* check_bounds
  *
  *    Purpose: check to make sure the height and width of the two images
  *             differs by no more than 1
  *
  * Parameters: the two ppm images
  *
  *    Returns: none 
  *
 */
 void check_bounds(Pnm_ppm image1, Pnm_ppm image2)
 {
     int h1 = image1->height;
     int h2 = image2->height;
     int w1 = image1->width;
     int w2 = image2->width;
     
     if (abs(h1 - h2) > 1 || abs(w1 - w2) > 1) {
             fprintf(stderr, "Images width or height differ by more than 1\n");
             printf("%lf\n", 1.0);
             exit(1);
     }
 }
 
 /* chooseHeight
  *
  *    Purpose: determine which height is lower of the two images 
  *
  * Parameters: two ppm images 
  *
  *    Returns: the lower of the two image heights 
  *
 */
 unsigned chooseHeight(Pnm_ppm image1, Pnm_ppm image2)
 {
      unsigned height = image1->height;
      
      if (image2->height < height) {
          height = image2->height;
      }
      
      return height;  
 }
 
 /* chooseWidth
  *
  *    Purpose: determine which width is lower of the two images 
  *
  * Parameters: two ppm images 
  *
  *    Returns: the lower of the two image widths 
  *
 */
 unsigned chooseWidth(Pnm_ppm image1, Pnm_ppm image2)
 {
      unsigned width = image1->width;
      
      if (image2->width < width) {
          width = image2->width;
      }
      
      return width;  
 }

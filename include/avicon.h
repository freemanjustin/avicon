// freeman.justin@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "md5.h"
#include "jutil.h"

#define fail(...) avicon_fail(__LINE__,__func__,__FILE__,__VA_ARGS__)

typedef struct {
    float r;
    float g;
    float b;
}rgb;

typedef struct{

  unsigned char  *data;  // image data
  int   h;  // height
  int   w;  // width
  int   n;  // number of color components

}image;


typedef struct {

    char*   input_string;
    char*   fname;

    image   five;
    image   cs;

    int     r;
    int     g;
    int     b;

    int     nBackgrounds;
    rgb     *background_colors;
    rgb     background;

    // for color channel shifting
    int iterations;

    // use result image as new source for iterations
    int recursiveIterations;

    // shift the image vertically true/false
    int shiftVertically;

    // shift the image horizontally true/false
    int shiftHorizontally;

    int horizontalShiftAmount;
    int verticalShiftAmount;

    int haveHs;
    int haveVs;

}e;

// prototypes
void avicon_fail( const int, const char*, const char*, const char*, ... );
void copyChannel(image *in, image *out, int sourceY, int sourceX, int sourceChannel, int targetChannel);
void copy_image(image *src, image *dest);

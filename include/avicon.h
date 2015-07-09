// freeman.justin@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "md5.h"
#include "jutil.h"

#define fail(...) avicon_fail(__LINE__,__func__,__FILE__,__VA_ARGS__)

typedef struct {

    char*   input_string;
    char*   fname;
    
    int     width;
    int     height;
    char*   image;
    
    int     r;
    int     g;
    int     b;
    

}e;

// prototypes
void avicon_fail( const int, const char*, const char*, const char*, ... );

// freeman.justin@gmail.com
#ifndef STB_HEADERS
#define STB_HEADERS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize.h"

#endif
#include "avicon.h"


// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;


	//min = MIN( r, g, b );
	//max = MAX( r, g, b );

    min = r < g ? r : g;
    min = min  < b ? min  : b;

    max = r > g ? r : g;
    max = max  > b ? max  : b;

    *v = max;				// v
	delta = max - min;
	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

//Adding HueShift
void HueShift(float *hue, float shift) {
    *hue -= shift;
    while (*hue >= 360.0) *hue -= 360.0;
    while (*hue < 0.0) *hue += 360.0;
}

double rand_in_range(double min, double max){
  return min + (max - min) * ((double)rand() / (double)RAND_MAX);
}

float randInRange(float min, float max)
{
  if (min==max) return 0.0;
  else return (float)( min + (rand() / (float) (RAND_MAX + 1.0) * (max - min)));
}

int main(int argc,char **argv)
{
    e	*E;
    int i,j;
    int ii,jj;
    int index;
    int pattern[5][5];

    
    int border;
    int pixel_size;

    int r;
    int di;

    float   red, green, blue;
    float   h_col, s_col, v_col;
    float   orig_s_col;

    float   rand_num;
    int     which_background;

    struct timeval t1;



    md5_state_t state;
    md5_byte_t digest[16];

    // set random number seed
    //srand(time(NULL));

    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);

	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL) fail("malloc failed\n");

	// parse command line arguments
	if(argc < 3){
		fail("need an input string file and an output png filename\n");
	}
	else{
		get_command_line_arg_as_string(&E->input_string, argv[1]);
		get_command_line_arg_as_string(&E->fname, argv[2]);

        printf("string is: %s\n", E->input_string);
	}

    // gene md5 hash

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)E->input_string, strlen(E->input_string));
	md5_finish(&state, digest);
	printf("MD5 (\"%s\") = \n", E->input_string);
	for (di = 0; di < 16; ++di)
	    printf("%d\t%02x\t%d\n",di, digest[di], digest[di]);
	printf("\n");

    // image dimensions
    E->five.w = 420;
    E->five.h = 420;
    E->five.n = 3;

    // image background
    // original
    //background = 255; //250 ;  // original is 240

    // allocate memory for the image data
    E->five.data = malloc(E->five.w*E->five.h*E->five.n*sizeof(unsigned char));
    if ( E->five.data == NULL) {
        fail("failed to allocate memory for image \n");
    }


    // set the pixel color using the first three values
    // from the md5 hash
    E->r = digest[13];
    E->g = digest[14];
    E->b = digest[15];

    E->nBackgrounds = 4;
    E->background_colors = malloc(E->nBackgrounds*sizeof(rgb));

    // define the background colors

    // white
    E->background_colors[0].r = 1.0;
    E->background_colors[0].g = 1.0;
    E->background_colors[0].b = 1.0;

    // warm white
    E->background_colors[1].r = 0.945;
    E->background_colors[1].g = 0.926;
    E->background_colors[1].b = 0.776;

    // black
    E->background_colors[2].r = 0.0;
    E->background_colors[2].g = 0.0;
    E->background_colors[2].b = 0.0;

    // complementary to color derived from MD5 hash
    RGBtoHSV( E->r/255.0, E->g/255.0, E->b/255.0, &h_col, &s_col, &v_col );
    HueShift(&h_col,180.0);
    HSVtoRGB(&E->background_colors[3].r, &E->background_colors[3].g, &E->background_colors[3].b, h_col, s_col, v_col);


    // set the background color for the output image
    which_background = (int)floor(rand_in_range(0.0,4.0));
    printf("which_background = %d\n", which_background);
    E->background.r = E->background_colors[which_background].r;
    E->background.g = E->background_colors[which_background].g;
    E->background.b = E->background_colors[which_background].b;

    // accessing 3d array as 1d
    // int* array = new int[ width * height * depth ];
    // array[ x * height * depth + y * depth + z ] = value;

    // background fill
    for(i=0;i<E->five.h;i++){
        for(j=0;j<E->five.w;j++){
            E->five.data[(i * E->five.w  + j) * E->five.n + 0] = E->background.r*255.0;;
            E->five.data[(i * E->five.w  + j) * E->five.n + 1] = E->background.g*255.0;;
            E->five.data[(i * E->five.w  + j) * E->five.n + 2] = E->background.b*255.0;;
            //E->five.data[(i * E->five.w  + j) * E->five.n + 3] = 255;
        }
    }

    // now fill in 70x70 pixel squares
    // each row has 5 squares
    // each column has 5 squares

    // we apply horizontal symmetry to the generated image such that
    // square [j][4] = [j][0]
    // square [j][3] = [j][1]
    // square [j][2] is the centre

    // do this per pixel
    // and then create the 420x420 image


    // working from this part down...

    r = rand() % 3; // get a number between 1 and 2
    printf("r = %d\n", r);
    index=0;
    for(j=0;j<5;j++){
        for(i=0;i<3;i++){

            if( r == 0 ){
                if(digest[index] % 2 == 0){
                    pattern[j][i] = 1;
                }
                else
                    pattern[j][i] = 0;
            }
            else if( r == 1 ){
                if(digest[index] % 2 != 0){
                    pattern[j][i] = 1;
                }
                else
                    pattern[j][i] = 0;
            }
            else if( r == 2){
                if(digest[index] < 128){
                    pattern[j][i] = 1;
                }
                else
                    pattern[j][i] = 0;
            }
            index++;
        }
    }

    // make the image symmetric
    for(j=0;j<5;j++){
        pattern[j][4] = pattern[j][0];
        pattern[j][3] = pattern[j][1];
    }

    // create the image (420x420 with 35 pixel border)
    border = 35;
    pixel_size = 70;

    rand_num = rand_in_range(0.0,1.0);
    //printf("rnum = %f\n", rand_num);
    if( rand_num > 0.2 ){
        rand_num = 1.0;
    }

    for(j=0;j<5;j++){
        for(i=0;i<5;i++){
            if(pattern[j][i] == 1){
                // experimental rgb -> hsv -> randomize over value -> rgb
                RGBtoHSV( E->r/255.0, E->g/255.0, E->b/255.0, &h_col, &orig_s_col, &v_col );
                
                if(rand_num == 1.0 ){    // i'm using rand_num like a boolean
                    s_col = orig_s_col + rand_in_range(-0.25,0.25);
                    if(s_col < 0.0) s_col = orig_s_col;
                    if(s_col > 1.0) s_col = orig_s_col;
                }
                
                
                HSVtoRGB(&red, &green, &blue, h_col, s_col, v_col);


                for(jj=(j*pixel_size)+border;jj<(j*pixel_size)+border+pixel_size;jj++){
                    for(ii=(i*pixel_size)+border;ii<(i*pixel_size)+border+pixel_size;ii++){
                        E->five.data[jj * E->five.w * E->five.n + ii * E->five.n + 0] = red*255.0;
                        E->five.data[jj * E->five.w * E->five.n + ii * E->five.n + 1] = green*255.0;
                        E->five.data[jj * E->five.w * E->five.n + ii * E->five.n + 2] = blue*255.0;
                    }
                }
            }
        }
    }

    // color channel shift if rnum > some_value
    rand_num = rand_in_range(0.0,1.0);
    //printf("rnum = %f\n", rand_num);
    if( rand_num > 0.5 ){   // 50 percent chance of colorshifting

        printf("colorshifting\n");
        // vars
        int sourceChannel;
        int targetChannel;

        // setup channel shifting 
        E->iterations = (int)floor(rand_in_range(2.0,5.0)); // either 0 to 3
        printf("  iterations = %d\n", E->iterations);

        // use result image as new source for iterations
        E->recursiveIterations = (int)floor(rand_in_range(0.0,1.0)); // either 0 or 1
        printf("  recursion = %d\n", E->recursiveIterations);
        // shift the image vertically true/false
        E->shiftVertically = 1;//(int)floor(rand_in_range(0.0,1.0)); // either 0 or 1
        printf("  shiftVertically = %d\n", E->shiftVertically);
        if(E->shiftVertically == 1){
            E->haveVs = 1;//(int)floor(rand_in_range(0.0,1.0)); // either 0 or 1
            if(E->haveVs == 1)
                E->verticalShiftAmount = (int)floor(rand_in_range(0,15)); // either 0 or 1;
        }

        // shift the image horizontally true/false
        E->shiftHorizontally = 1;//(int)floor(rand_in_range(0.0,1.0)); // either 0 or 1;
        printf("  shiftHorizontally = %d\n", E->shiftHorizontally);
        if(E->shiftHorizontally == 1)
            E->haveHs = 1;//(int)floor(rand_in_range(0.0,1.0)); // either 0 or 1
            if(E->haveHs == 1)
                E->horizontalShiftAmount = (int)floor(rand_in_range(0,15)); // either 0 or 1;


        // get our colorshifted pixel space ready
        E->cs.w = E->five.w;
        E->cs.h = E->five.h;
        E->cs.n = 3;
        E->cs.data = (unsigned char*)malloc(E->cs.w*E->cs.h*E->cs.n*sizeof(unsigned char));
        // copy input image into colorshifted data array for messing with later
        // copy)image(src, dest);
        copy_image(&E->five, &E->cs);

        for(i = 0; i < E->iterations; i++){
            // generate random numbers
            // for which channels to swap
            sourceChannel = (int)randInRange(0,3);
            //sourceChannel = 1;    // i sometimes set this manually
            do{
                targetChannel = (int)randInRange(0,3);
                //targetChannel = 0;    // i sometimes set this manually
            }while(targetChannel == sourceChannel);

            printf("sourceChannel = %d, targetChannel = %d\n", sourceChannel, targetChannel);

            // start with no horizontalShift
            int horizontalShift = 0;

            // if shiftHorizontally is true, generate a random number to shift horizontally by
            if(E->shiftHorizontally){
                horizontalShift = (int)randInRange(0,E->cs.w);
                // tiny horizontal displacements
                if(E->haveHs == 1){
                    //do{
                        horizontalShift = (int)randInRange(-E->horizontalShiftAmount,E->horizontalShiftAmount); // i sometimes set this manually
                    //}while(horizontalShift == 0);
                }
                printf("horizontalShift = %d\n", horizontalShift);
            }

            // start with no verticalShift
            int verticalShift = 0;

            // if shiftVertically is true, generate a random number to shift vertically by
            if(E->shiftVertically){
                verticalShift = (int)randInRange(0,E->cs.h);
                // tiny displacements
                if(E->haveVs == 1){
                    //do{
                        verticalShift = (int)randInRange(-E->verticalShiftAmount,E->verticalShiftAmount); // i sometimes set this manually
                    //}while(verticalShift == 0);
                }
                printf("shifting vertically by %d pixels\n", verticalShift);
            }

            // shift the channel
            copyChannel(&E->five, &E->cs, verticalShift, horizontalShift, sourceChannel, targetChannel);

            // use the target as the new source for the next iteration
            if(E->recursiveIterations){
                // copy the target to the source
                // copy_image(src, destination)
                printf("recursive is true ... doing copy_image()\n");
                copy_image(&E->cs, &E->five);
            }
        }

        // copy the imahe back to the five data structure
        copy_image(&E->cs, &E->five);
    }
    
    // apply postprocessing filter




    // write the image
    stbi_write_png(E->fname, E->five.w, E->five.h, E->five.n, (const void *)E->five.data, 0);

	return 0;
}

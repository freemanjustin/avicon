// freeman.justin@gmail.com

#include "avicon.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

int main(int argc,char **argv)
{
    e	*E;
    int i,j;
    int ii,jj;
    int index;
    int pattern[5][5];

    int background;
    int border;
    int pixel_size;

    int r;
    int di;

    float   red, green, blue;
    float   h_col, s_col, v_col;

    float   rand_num;

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
    E->width = 420;
    E->height = 420;

    // image background
    // original
    //background = 255; //250 ;  // original is 240

    // allocate memory for the image data
    if ((E->image = (char *) malloc(E->height*E->width*3*sizeof(char)) ) == NULL) {
        fail("failed to allocate memory for image \n");
    }


    // set the pixel color using the first three values
    // from the md5 hash
    E->r = digest[13];
    E->g = digest[14];
    E->b = digest[15];

    if( rand_in_range(0.0,1.0) > 0.75 ){
        // determine a complementary color to the pixel color derived from the
        // MD5 hash
        RGBtoHSV( E->r/255.0, E->g/255.0, E->b/255.0, &h_col, &s_col, &v_col );
        HueShift(&h_col,180.0);
        HSVtoRGB(&E->background.r, &E->background.g, &E->background.b, h_col, s_col, v_col);
    }
    else{
	// warm white #F1ECC6 --> 241,236,198 --> 0.945, 0.926, 0.776

        E->background.r = 0.945;
        E->background.g = 0.926;
        E->background.b = 0.776;

	E->background.r = 1.0;
        E->background.g = 1.0;
        E->background.b = 1.0;
    }

    // accessing 3d array as 1d
    // int* array = new int[ width * height * depth ];
    // array[ x * height * depth + y * depth + z ] = value;

    // background fill
    for (j=0;j<E->height;j++) {
        for (i=0;i<E->width;i++) {
            E->image[j * E->height * 3 + i * 3 ] = E->background.r*255.0;
            E->image[j * E->height * 3 + i * 3 + 1] = E->background.g*255.0;
            E->image[j * E->height * 3 + i * 3 + 2] = E->background.b*255.0;
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
    r = rand() % 3; // get a number between 1 and 2
    r = 0;
    //printf("r = %d\n", r);
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
    if( rand_num > 0.001 ){
        rand_num = 1.0;
    }

    for(j=0;j<5;j++){
        for(i=0;i<5;i++){
            if(pattern[j][i] == 1){
                // experimental rgb -> hsv -> randomize over value -> rgb
                RGBtoHSV( E->r/255.0, E->g/255.0, E->b/255.0, &h_col, &s_col, &v_col );
                if(rand_num == 1.0 ){    // i'm using rand_num like a boolean
                    s_col = s_col + rand_in_range(-0.3,0.3);
                    if(s_col < 0.0) s_col = 0.0;
                    if(s_col>1.0)s_col = 1.0;
                }
                HSVtoRGB(&red, &green, &blue, h_col, s_col, v_col);


                for(jj=(j*pixel_size)+border;jj<(j*pixel_size)+border+pixel_size;jj++){
                    for(ii=(i*pixel_size)+border;ii<(i*pixel_size)+border+pixel_size;ii++){
                        E->image[jj * E->height * 3 + ii * 3 + 0] = red*255.0;
                        E->image[jj * E->height * 3 + ii * 3 + 1] = green*255.0;
                        E->image[jj * E->height * 3 + ii * 3 + 2] = blue*255.0;
                    }
                }
            }
        }
    }

    // write the image
    stbi_write_png(E->fname, E->width, E->height, 3, (const void *)E->image, 0);

	return 0;
}

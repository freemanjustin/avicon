// freeman.justin@gmail.com

#include "avicon.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc,char **argv)
{
    e	*E;
    int i,j;
    int ii,jj;
    int index;
    int pattern[5][5];
    
    int border;
    int pixel_size;
    
    md5_state_t state;
    md5_byte_t digest[16];

    
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
	}
	
    // gene md5 hash
    
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)E->input_string, strlen(E->input_string));
	md5_finish(&state, digest);
	//printf("MD5 (\"%s\") = ", E->input_string);
	//for (di = 0; di < 16; ++di)
	//    printf("%02x\t%d\n", digest[di], digest[di]);
	//printf("\n");
    
    // image dimensions
    E->width = 420;
    E->height = 420;
    
    // allocate memory for the image data
    if ((E->image = (char *) malloc(E->height*E->width*3*sizeof(char)) ) == NULL) {
        fail("failed to allocate memory for image \n");
    }

    
    // accessing 3d array as 1d
    // int* array = new int[ width * height * depth ];
    // array[ x * height * depth + y * depth + z ] = value;
    
    // background fill
    for (j=0;j<E->height;j++) {
        for (i=0;i<E->width;i++) {
            for(index=0;index<3;index++) {
                E->image[j * E->height * 3 + i * 3 + index] = 240;
            }
        }
    }
    
    // set the pixel color using the first three values
    // from the md5 hash
    E->r = digest[0];
    E->g = digest[1];
    E->b = digest[2];
    
    // now fill in 70x70 pixel squares
    // each row has 5 squares
    // each column has 5 squares
    
    // we apply horizontal symmetry to the generated image such that
    // square [j][4] = [j][0]
    // square [j][3] = [j][1]
    // square [j][2] is the centre

    // do this per pixel
    // and then create the 420x420 image
    index=0;
    for(j=0;j<5;j++){
        for(i=0;i<3;i++){
            if(digest[index] % 2 != 0){
                pattern[j][i] = 1;
            }
            else
                pattern[j][i] = 0;
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
    
    for(j=0;j<5;j++){
        for(i=0;i<5;i++){
            if(pattern[j][i] == 1){
                for(jj=(j*pixel_size)+border;jj<(j*pixel_size)+border+pixel_size;jj++){
                    for(ii=(i*pixel_size)+border;ii<(i*pixel_size)+border+pixel_size;ii++){
                        E->image[jj * E->height * 3 + ii * 3 + 0] = E->r;
                        E->image[jj * E->height * 3 + ii * 3 + 1] = E->g;
                        E->image[jj * E->height * 3 + ii * 3 + 2] = E->b;
                    }
                }
            }
        }
    }
    
    // write the image
    stbi_write_png(E->fname, E->width, E->height, 3, (const void *)E->image, 0);
    
	return 0;
}
    

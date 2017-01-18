#include "avicon.h"

// ported from the processing script
void copyChannel(image *in, image *out, int sourceY, int sourceX, int sourceChannel, int targetChannel){

    int x, y;

    int sourceYOffset;
    int sourceXOffset;

    int sourceRed;
    int sourceGreen;
    int sourceBlue;

    int targetRed;
    int targetGreen;
    int targetBlue;

    int sourceChannelValue;

    // starting at the sourceY and pointerY loop through the rows
    for(y = 0; y < out->h; y++)
    {
        // add y counter to sourceY
        sourceYOffset = sourceY + y;

        // wrap around the top of the image if we've hit the bottom
        if(sourceYOffset >= out->h)
          sourceYOffset -= out->h;

        // wrap around the bottom of the image if we've hit the top
        if(sourceYOffset <0)
          sourceYOffset += out->h;

        // starting at the sourceX and pointerX loop through the pixels in this row
        for(x = 0; x < out->w; x++)
        {
            // add x counter to sourceX
            sourceXOffset = sourceX + x;

            // wrap around the left side of the image if we've hit the right side
            if(sourceXOffset >= out->w)
              sourceXOffset -= out->w;

             // wrap around the right side of the image if we've hit the left side
            if(sourceXOffset < 0)
              sourceXOffset += out->w;

            // get the RGB values of the source pixel
            sourceRed = in->data[(sourceYOffset * in->w  + sourceXOffset) * in->n + 0]; //red(sourcePixel);
            sourceGreen = in->data[(sourceYOffset * in->w  + sourceXOffset) * in->n + 1]; //green(sourcePixel);
            sourceBlue = in->data[(sourceYOffset * in->w  + sourceXOffset) * in->n + 2]; //blue(sourcePixel);

            // create a variable to hold the new source RGB channel value
            sourceChannelValue = 0;
            // assigned the source channel value based on sourceChannel random number passed in
            switch(sourceChannel){
              case 0:
                // use red channel from source
                sourceChannelValue = sourceRed;
                break;
              case 1:
                // use green channel from source
                sourceChannelValue = sourceGreen;
                break;
              case 2:
                // use blue channel from source
                sourceChannelValue = sourceBlue;
                break;
            }

            // assigned the source channel value to a target channel based on targetChannel random number passed in
            switch(targetChannel)
            {
              case 0:
                // assign source channel value to target red channel
                out->data[(y * out->w  + x) * out->n + 0] = sourceChannelValue;
                break;
              case 1:
                // assign source channel value to target green channel
                out->data[(y * out->w  + x) * out->n + 1] = sourceChannelValue;
                break;
              case 2:
                // assign source channel value to target blue channel
                out->data[(y * out->w  + x) * out->n + 2] = sourceChannelValue;
                break;
            }
        }
    }
    //stbi_write_png("colorshifted.png", out->w, out->h, out->n, out->data, 0);
    //exit(0);
}

void copy_image(image *src, image *dest){

    int i, j;

    // no error checking! asusmes that the src and destination are allocated and the same size!!!!
    for(i=0;i<src->h;i++){
        for(j=0;j<src->w;j++){
            dest->data[(i * dest->w  + j) * dest->n + 0] = src->data[(i * src->w  + j) * src->n + 0];
            dest->data[(i * dest->w  + j) * dest->n + 1] = src->data[(i * src->w  + j) * src->n + 1];
            dest->data[(i * dest->w  + j) * dest->n + 2] = src->data[(i * src->w  + j) * src->n + 2];
            //dest->data[(i * dest->w  + j) * dest->n + 3] = src->data[(i * src->w  + j) * src->n + 3];
        }
    }
}

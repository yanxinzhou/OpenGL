#include "Paluka-Infinite_Scene.h"



// load a RGB .RAW file as a texture
GLuint loadTextureRAW( const char * filename, int width, int height, int size)
{
    GLuint texture;
    BYTE* data;
    FILE* file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    // allocate buffer

    data = (BYTE*) malloc( width * height * size );

    // read texture data
    fread( data, size * width * height, 1, file );
    fclose( file );

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



    // build our texture mipmaps
	if (size == 4){
		gluBuild2DMipmaps( GL_TEXTURE_2D, size, width, height,
			GL_RGBA, GL_UNSIGNED_BYTE, data );
	} else if (size == 3){
		gluBuild2DMipmaps( GL_TEXTURE_2D, size, width, height,
			GL_RGB, GL_UNSIGNED_BYTE, data );
	}

    // free buffer
    free( data );

    return texture;
}

void readBackBuffer(char* fName, int startX, int startY, int width, int height){
        FILE *file;
		BYTE* image;
        

        /* Allocate our buffer for the image */
        image = (BYTE*) malloc(4*width*height);

        file = fopen(fName,"wb");
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
        /* Copy the image into our buffer */
        glReadBuffer(GL_BACK_LEFT);
        glReadPixels(startX, startY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

		for (int i = 0; i < (width * height * 4); i = i + 4){
			if (image[i] == 0 && image[i+1] == 0 && image[i+2] == 0){
				image[i+3] = 0;
			}
		}

        /* Write the raw file */
        fwrite(image, width * height * 4, 1, file);       

        fclose(file);
		free(image);
}
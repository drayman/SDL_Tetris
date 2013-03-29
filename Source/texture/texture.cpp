#include <stdexcept>

#include "texture.h"
#include "log/log.h"

void Texture::reset()
{
    clear_image_data();

    fileName = "[Empty texture]";
    Bpp = 0; width = 0; height = 0;
    type = GL_UNSIGNED_BYTE; format = GL_RGB;

    if ( initialized ) {
        initialized = false;
        glDeleteTextures( 1, &texID );
        texID = 0;
    }
}


void Texture::bind() {
    if (initialized) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, texID );
    } else
        glDisable(GL_TEXTURE_2D);
}


void Texture::build()
{
    // since we have no extension handling and environment information
    // we calculate GL_VERSION in place
    int gl_major = 0, gl_minor = 0;

    const char *verstr = (const char *) glGetString(GL_VERSION);
#   ifndef DESKTOP_DETECTED
    char strdummy[30];
#   endif

    if ((verstr == NULL) ||
#       ifdef DESKTOP_DETECTED
        (sscanf(verstr,"%d.%d", &gl_major, &gl_minor) != 2 ) ||
#       else
        (sscanf(verstr,"OpenGL %s %d.%d", &strdummy, &gl_major, &gl_minor) != 3 ) ||
#       endif
        (gl_major == 0 ))
        throw std::runtime_error("OpenGL not initialized before building texture");

    // If exists, erase previous texture
    if ( texID > 0 ) glDeleteTextures( 1, &texID );


    // Check that the image's width is a power of 2   :)
    if ((width & (width - 1)) != 0  || (width & (width - 1)) != 0 )
    {
        /// TODO check if NON_POWER_OF_TWO extension supported
        /// simply strstr(), glGetString(GL_EXTENSIONS)...
        throw std::runtime_error("Width/Height is not a power of 2");
    }

    // It there's data try to build the GL texture
    if ( imageData )
    {
        // check for previous errors
        for (GLint error = glGetError(); error; error = glGetError())
            LOGW("glError (0x%x) collected before building texture", error);

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texID );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texID );

        // Set the texture's stretching properties
#       ifdef DESKTOP_DETECTED  // auto mipmap generation is supported from GL 1.4 and from GLES 1.1
        if (gl_major > 1 || gl_minor > 3) {
#       else
        if (gl_major > 1 || gl_minor > 0) {
#       endif

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
            glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        } else {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Edit the texture object's image data using the information held by our structure
#       ifdef DESKTOP_DETECTED
        glTexImage2D( GL_TEXTURE_2D, 0, Bpp, width, height, 0, format, type, imageData );
#       else
        glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, type, imageData );
#       endif

        // Check if any error occured
        bool error_occured = false;
        while (glGetError() != GL_NO_ERROR) {error_occured = true;}
        if (error_occured) throw std::runtime_error("Error while building texture: "+fileName);

    } else  // throw an exception
        throw std::runtime_error("Image data is null while trying to build texture from "+fileName);

    LOGI("Texture #%d initialized (%s)", texID, fileName.c_str());
    initialized = true;
}


void Texture::swap_BGR_RGB()
{
    if (!imageData || Bpp == 0 || width == 0 || height == 0) return;

    // inverse B and R
    for (unsigned int i=0; i<(width * height * Bpp); i+=Bpp)
        // 1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte :)
        imageData[i] ^= imageData[i+2] ^= imageData[i] ^= imageData[i+2];
}


void Texture::flip_y()
{
    if (!imageData || Bpp == 0 || width == 0 || height == 0) return;

    unsigned char temp[Bpp];

    for (unsigned int w = 0; w < width; w++)
    for (unsigned int h = 0; h < height / 2; h++) {
        memcpy(temp, &imageData[(h*width+w)*Bpp], Bpp);
        memcpy(&imageData[(h*width+w)*Bpp], &imageData[((height-h-1)*width+w)*Bpp], Bpp);
        memcpy(&imageData[((height-h-1)*width+w)*Bpp], temp, Bpp);
    }
}

void swap(unsigned char * ori, unsigned char * dest, GLint size){
    GLubyte * temp = new unsigned char[size];

    memcpy(temp, ori, size);
    memcpy(ori, dest, size);
    memcpy(dest, temp, size);

   delete [] temp;
}


Texture::~Texture() {
    if ( texID > 0 ) glDeleteTextures( 1, &texID );
    LOGI("Texture #%d cleared (%s)", texID, fileName.c_str() );
}


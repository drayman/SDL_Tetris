#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>

#include <stdexcept>

#include "TGA_texture.h"

TGA_Texture::TGA_Texture( const std::string& file_name ) {
    load( file_name );
    build();
}


void TGA_Texture::load( const std::string& file_name )
{
    if (initialized) reset();

    if (file_name.size()<5) throw std::runtime_error("Invalid TGA filename");
    fileName = file_name;

	SDL_RWops * fTGA = NULL;                                                    // File pointer to texture file

    try {
        fTGA = SDL_RWFromFile(fileName.c_str(), "rb");                          // Open file for reading
        if(fTGA == NULL)                                                        // If it didn't open....
            throw std::runtime_error("Couldn't open");

        /// http://www.paulbourke.net/dataformats/tga/
        GLubyte TGAInfoHeader[18];                                              // TGA header contains type info
        if(SDL_RWread(fTGA, TGAInfoHeader, 18, 1) == 0)                         // Attempt to read 18 byte header from file
            throw std::runtime_error("Couldn't read header");

        width  = TGAInfoHeader[12] + (TGAInfoHeader[13] << 8);                  // Determine The TGA Width  (highbyte*256+lowbyte)
        height = TGAInfoHeader[12] + (TGAInfoHeader[15] << 8);                  // Determine The TGA Height (highbyte*256+lowbyte)
        Bpp	   = TGAInfoHeader[16] / 8;                                         // Determine the bits per pixel
        bool inverted_y = (TGAInfoHeader[17] & 0x20) == 0;                      // Check if Bottom-Left origin is used

        if((width == 0) || (height == 0) || ((Bpp != 3) && (Bpp != 4)))         // Make sure all information is valid
            throw std::runtime_error("Invalid texture information");

        if(Bpp == 3) {                                                          // Set BPP
#       ifdef GL_BGR                                                            // I dunno whether BGR and BGRA could seperately exist
            format = GL_BGR;                                                    // To make sure we don't check against BGR and then
#       else                                                                    // use BGRA for example, i set up the format flag here
            format = GL_RGB;
#       endif
        } else {
#       ifdef GL_BGRA
            format = GL_BGRA;
#       else
            format = GL_RGBA;
#       endif
        }

        if (TGAInfoHeader[0] > 0 )                                              // Hell, we have found and ancient identification field!
            if (SDL_RWseek(fTGA, TGAInfoHeader[0], SEEK_CUR)                    // Leave this interesting prehistorical comment tag :(
                                   != TGAInfoHeader[0] + 18)                    // Check if seek was successfull - NEED TESTING
                throw std::runtime_error("Couldn't seek");

        GLuint imageSize = Bpp * width * height;                                // Compute the total amount of memory needed to store data
        imageData = (GLubyte*) malloc(imageSize);		                        // Allocate that much memory

        if(imageData == NULL)                                                   // If no space was allocated
            throw std::runtime_error("Couldn't allocate memory");

        if(TGAInfoHeader[2] == 2)                                               // Check if our TGA is uncompressed RGB
        {
            unsigned char colorbuffer[4];                                       // Storage for 1 pixel
            int x, y;

            for (y = (inverted_y ? (height-1) : 0);                             // Loop for reading image data
                     (inverted_y ? (y >= 0)   : (y < (int)height));             // and perform y-flip, if needed
                     (inverted_y ? (--y)      : (++y)))
            for (x = 0; x < (int)width; x++) {

                if (SDL_RWread(fTGA, colorbuffer, 1, Bpp) != Bpp)               // Get the next pixel
                    throw std::runtime_error("Couldn't ttt read");

                int j = (y * width * Bpp) + (x * Bpp);

                if (format==GL_RGB || format==GL_RGBA) {                        // Save pixel data, and change R and B
                    imageData[j  ] = colorbuffer[2];                            // values if BGR or BGRA is not recognized
                    imageData[j+1] = colorbuffer[1];
                    imageData[j+2] = colorbuffer[0];
                } else {
                    imageData[j  ] = colorbuffer[0];
                    imageData[j+1] = colorbuffer[1];
                    imageData[j+2] = colorbuffer[2];
                }

                if(Bpp == 32) imageData[j+3] = colorbuffer[3];                  // Add extra byte of alpha if image is 32bit
            }
        }
        else if (TGAInfoHeader[2] == 10)                                        // Check if our TGA is RLE compressed
        {
            GLuint pixelcount	= height * width;
            GLuint currentpixel	= 0;
            GLuint currentbyte	= 0;

            do {
                GLubyte chunkheader = 0;                                        // Storage for "chunk" header

                if(SDL_RWread(fTGA, &chunkheader, sizeof(GLubyte), 1) == 0)     // Read in the 1 byte header
                    throw std::runtime_error("Could not read RLE header");

                if(chunkheader < 128)                                           // If the header is < 128, that is the number of RAW color packets minus 1
                {                                                               // that follow the header
                    chunkheader++;                                              // add 1 to get number of following color values
                    for(GLubyte counter = 0; counter < chunkheader; counter++)  // Read RAW color values
                    {
                        if (SDL_RWread(fTGA, &imageData[currentbyte], 1, Bpp) != Bpp)
                            throw std::runtime_error("Couldn't read");          // Try to store 1 pixel

                        currentbyte += Bpp;                                     // Increase thecurrent byte by the number of bytes per pixel
                        currentpixel++;											// Increase current pixel by 1

                        if(currentpixel > pixelcount)                           // Make sure we havent read too many pixels
                            throw std::runtime_error("Too many pixels read");
                    }
                }
                else                                                            // chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
                {
                    unsigned char colorbuffer[4];                               // Storage for 1 pixel
                    chunkheader -= 127;                                         // Subteact 127 to get rid of the ID bit
                    if (SDL_RWread(fTGA, colorbuffer, 1, Bpp) != Bpp)           // Attempt to read following color values
                        throw std::runtime_error("Couldn't read");

                    for (GLubyte counter = 0; counter < chunkheader; counter++) // copy the color into the image data as many times as dictated
                    {                                                           // by the header
                        memcpy(&imageData[currentbyte], colorbuffer, Bpp);

                        currentbyte += Bpp;                                     // Increase current byte by the number of bytes per pixel
                        currentpixel++;                                         // Increase pixel count by 1

                        if(currentpixel > pixelcount)                           // Make sure we havent written too many pixels
                            throw std::runtime_error("Too many pixels read");
                    }
                }
            }
            while(currentpixel < pixelcount);                                   // Loop while there are still pixels left

            if (format== GL_RGB || format == GL_RGBA)                           // Swap color pixels if BGR or BGRA are'nt supported
                swap_BGR_RGB();

            if ( inverted_y ) flip_y();                                         // We should rotate it; This part is really unoptimized...

        }
        else                                                                    // If header matches neither type
        {
            throw std::runtime_error("Unsupported TGA");                        // Throw exception
        }

        SDL_RWclose(fTGA);                                                      // Close file

    }
    catch (...) {
		if(fTGA != NULL) SDL_RWclose(fTGA);                                     // If file is still open close it
        reset();                                                                // Free allocated memory and load default values
        throw;
    }

}


void TGA_Texture::clear_image_data()
{
    // Check if the data exists
    if (!imageData) return;

    // Perform cleaning
    free(imageData);
    imageData = NULL;
}


TGA_Texture::~TGA_Texture() {
    clear_image_data();
}



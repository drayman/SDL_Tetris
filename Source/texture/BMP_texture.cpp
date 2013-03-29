#include <stdexcept>

#include "BMP_texture.h"


BMP_Texture::BMP_Texture( const std::string& file_name ) {
    load( file_name );
    build();
}


void BMP_Texture::load( const std::string& file_name )
{
    if (initialized) reset();

    if (file_name.size()<5) throw std::runtime_error("Invalid BMP filename.");
    fileName = file_name;

    // This surface will tell us the details of the image
    if ( !(SDL_surface = SDL_LoadBMP( fileName.c_str())) )
        throw std::runtime_error(std::string(SDL_GetError()));
    else
    {
        try
        {
            // assign the raw image data to the texture
            imageData = (GLubyte*)SDL_surface->pixels;

            width = SDL_surface->w;
            height = SDL_surface->h;

            // get the number of channels in the SDL surface
            Bpp = SDL_surface->format->BytesPerPixel;
            if (Bpp == 4) {    // contains an alpha channel
                    if (SDL_surface->format->Rmask == 0x000000ff)
                        format = GL_RGBA;
                    else {
#                       ifdef GL_BGRA
                        format = GL_BGRA;
#                       else
                        swap_BGR_RGB();
                        format = GL_RGBA;
#                       endif
                    }
            } else if (Bpp == 3) {    // no alpha channel
                    if (SDL_surface->format->Rmask == 0x000000ff)
                        format = GL_RGB;
                    else
#                       ifdef GL_BGR
                        format = GL_BGR;
#                       else
                        swap_BGR_RGB();
                        format = GL_RGB;
#                       endif
            } else {
                /// TODO Support for popular RGB_565
                throw std::runtime_error("Not truecolor");
            }
        }
        catch (...) {
            reset();
            throw;
        }
    }
}


void BMP_Texture::clear_image_data()
{
    // Check if the data exists
    if (!imageData) return;

    // Perform cleaning
    SDL_FreeSurface( SDL_surface );
    imageData = NULL;
}


BMP_Texture::~BMP_Texture() {
    clear_image_data();
}



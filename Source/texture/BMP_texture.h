#ifndef BMP_TEXTURE_H
#define BMP_TEXTURE_H

#include "texture.h"

class BMP_Texture : public Texture {

public:

    /// Constructor to create default/empty BMP texture
    BMP_Texture() {}

    /// Constructor to create texture from BMP image file
    BMP_Texture( const std::string& file_name );

    /// BMP loader implementation based on SDL_Surface
    virtual void load( const std::string& file_name );

    /// Frees the needless raw data of the image by freeing the attached SDL_surface
    virtual void clear_image_data();

    /// Stores BMP-specific information and image data
    SDL_Surface* SDL_surface;

    /// Destructor for type specific cleaning
    /// It unregisters the GL texture, and calls clear_image_data()
    virtual ~BMP_Texture();

};


#endif // BMP_TEXTURE_H

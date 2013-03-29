#ifndef TGA_TEXTURE_H
#define TGA_TEXTURE_H

#include "texture.h"

class TGA_Texture : public Texture {

public:

    /// Constructor to create default/empty TGA texture
    TGA_Texture() {}

    /// Constructor to create texture from TGA image file
    TGA_Texture( const std::string& file_name );

    /// A simple TGA loader implementation
    virtual void load( const std::string& file_name );

    /// Frees the needless raw data of the image
    virtual void clear_image_data();

    /// Destructor for type specific cleaning
    /// It unregisters the GL texture, and calls clear_image_data()
    virtual ~TGA_Texture();

};

#endif // TGA_TEXTURE_H

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "system/resource_manager.h"
#include "BMP_texture.h"
#include "TGA_texture.h"

class TextureManager : public ResourceManager<Texture> {

    virtual Resource load( const std::string& key );

public:

    void bind( const std::string& key );    ///< Activates the GL texture

    std::string respath;                    ///< If the path of the resource is non-standard
    std::string prefix;                     ///< Used for smaller textures on compact devices
};

#endif // TEXTURE_MANAGER_H

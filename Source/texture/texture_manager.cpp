#include <stdexcept>

#include "texture_manager.h"
#include "log/log.h"


ResourceManager<Texture>::Resource TextureManager::load(const std::string& key)
{
    if (key.size()<5) throw std::runtime_error("Invalid image filename.");

    std::string extension = key.substr(key.size()-4,4);
    for(unsigned int i=0; i < extension.size(); i++)
        extension[i]=tolower(extension[i]);

    const std::string fullname = respath+prefix+key;
    TextureManager::Resource texture;

    try {
        if ( extension.compare(".bmp") == 0 ) {
            texture = TextureManager::Resource( new BMP_Texture( fullname ) );
        } else
        if ( extension.compare(".tga") == 0 ) {
            texture = TextureManager::Resource( new TGA_Texture( fullname ) );
        } else throw std::runtime_error(fullname+"Not supported image format.");

    } catch (std::runtime_error e) {
        // don't throw the exception, we return a default texture
        LOGW("%s (%s)",e.what(), fullname.c_str());
        texture = TextureManager::Resource(new TGA_Texture);
        texture->fileName = "[Broken texture]" + fullname;
    }
    // we could keep the raw data, but this time we don't need it.
    texture->clear_image_data();

    return texture;
}

void TextureManager::bind(const std::string& key)
{
    if ( resources.count(key) == 0 ) resources[key] = load(key);
    resources[key]->bind();
}



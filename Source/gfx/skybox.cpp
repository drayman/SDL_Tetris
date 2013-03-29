#include "skybox.h"


Skybox::Skybox(bool draw_normals, bool draw_colors, bool draw_textures )
{
    setProperties(draw_normals, draw_colors, draw_textures);
}


void Skybox::setTextures(
    std::shared_ptr<Texture> south_tex,
    std::shared_ptr<Texture> north_tex,
    std::shared_ptr<Texture> west_tex,
    std::shared_ptr<Texture> east_tex,
    std::shared_ptr<Texture> top_tex,
    std::shared_ptr<Texture> bottom_tex)
{
    textures[0] = south_tex;
    textures[1] = north_tex;
    textures[2] = west_tex;
    textures[3] = east_tex;
    textures[4] = top_tex;
    textures[5] = bottom_tex;
}


void Skybox::setProperties(bool draw_normals, bool draw_colors, bool draw_textures)
{
    normal = draw_normals;
    color = draw_colors;
    texture = draw_textures;
}


void Skybox::draw()
{
    for (unsigned int i = 0; i<6; i++)
    if (textures[i] != NULL && textures[i]->isInitialized())
    {
        textures[i]->bind();
        skybox.draw(normal, color, texture, i*6, 6 );
    }
}

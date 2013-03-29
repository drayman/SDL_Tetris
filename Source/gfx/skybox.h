#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture/texture.h"
#include "mesh/skybox_mesh.h"

#include <memory>

class Skybox
{

public:

    Skybox(
        bool draw_normals  = true,
        bool draw_colors   = true,
        bool draw_textures = true
    );

    /// Assign the textures
    void setTextures(
        std::shared_ptr<Texture> south_tex,
        std::shared_ptr<Texture> north_tex,
        std::shared_ptr<Texture> west_tex,
        std::shared_ptr<Texture> east_tex,
        std::shared_ptr<Texture> top_tex,
        std::shared_ptr<Texture> bottom_tex=std::shared_ptr<Texture>()
    );

    /// Draws the skybox
    void draw();

    /// Set drawing of normals, colors, and textures
    void setProperties(bool draw_normals, bool draw_colors, bool draw_textures);

private:

    SkyboxMesh skybox;

    std::shared_ptr<Texture> textures[6];

    bool normal, color, texture;

};

#endif // SKYBOX_H

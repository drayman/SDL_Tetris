#ifndef BRICK_H
#define BRICK_H

#include "animation/animation.h"
#include "mesh/cube_mesh.h"

#include <memory>
#include "texture/texture.h"

/**
    Tetris brick struct

    This struct intended to be an as minimal as possible representation of
    a brick object. It only stores brick attributes
*/

struct Brick
{
    /// Constructor to build a fully initialized cube
    Brick
    (
        float color_r, float color_g, float color_b, float color_a,
        std::shared_ptr<Texture> shared_texture
    ) :
        texture_ptr(shared_texture)

    {
        this->color_r=color_r;
        this->color_g=color_g;
        this->color_b=color_b;
        this->color_a=color_a;
        this->texture_ptr=texture_ptr;
    }

    /// Constructor to build an empty cube
    Brick()
    {
        this->color_r=0.0f;
        this->color_g=0.0f;
        this->color_b=0.0f;
        this->color_a=0.0f;
    };

    std::unique_ptr<Animation> leftAnim;
    std::unique_ptr<Animation> rightAnim;
    std::unique_ptr<Animation> downAnim;
    std::unique_ptr<Animation> rotateAnim;
    std::unique_ptr<Animation> alphaAnim;

    std::shared_ptr<Texture> texture_ptr;

    float color_r;
    float color_g;
    float color_b;
    float color_a;

};

#endif // BRICK_H

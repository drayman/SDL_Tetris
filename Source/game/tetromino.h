#ifndef TETROMINO_H
#define TETROMINO_H

#include "game/brick.h"
#include "texture/texture_manager.h"
#include "mesh/cube_mesh.h"

#include <memory>


class TetroFactory;

class Tetromino
{

public:

    unsigned char shape_size;
    std::unique_ptr<Brick> shape[4][4];
    float center_x;
    float center_y;

private:

    Tetromino() {};

friend class TetroFactory;

};


class TetroFactory
{

public:

    TetroFactory
    (
        TextureManager& texture_manager
    ) :
        texmanager(texture_manager)
    {}

    std::unique_ptr<Tetromino> createTetro(bool extended);

private:

    TextureManager& texmanager;

};


#endif // TETROMINO_H

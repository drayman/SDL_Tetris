#ifndef TETROMINO_H
#define TETROMINO_H

#include "game/brick.h"
#include "texture/texture_manager.h"
#include "mesh/cube_mesh.h"

#include <memory>

#define NUMBER_OF_TETROMINOS 7

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
    {
        reset();
    }

    std::unique_ptr<Tetromino> createTetro(bool extended);

    void reset();

private:

    TextureManager& texmanager;

    unsigned int tetro_count[NUMBER_OF_TETROMINOS];
    unsigned int tetro_sum;

};


#endif // TETROMINO_H

#include "texture/texture_manager.h"

#include "game/tetromino.h"

/*
    OpenGL use:       C-arrays are:

        ^                   y
      y |                 +--->
        +--->           x |
          x               v

    To keep the design simple, i use GL style,
    and replace x and y later in drawing then
    process the models with inverted y axis...
*/

std::unique_ptr<Tetromino> TetroFactory::createTetro(bool extended)
{
    /// Create the product
    //auto tetro_ptr = std::make_unique<Tetromino>();
    std::unique_ptr<Tetromino> tetro_ptr(new Tetromino());


    /// Random generator balanced algorythm
    unsigned int tetro_max = tetro_count[0],
                 tetro_id = 0;

    for (unsigned int i = 1; i < NUMBER_OF_TETROMINOS; i++) {
        if (tetro_count[i]>tetro_max) tetro_max = tetro_count[i];
    }

    const unsigned int DELTA = 2;

    unsigned int interval = (tetro_max + DELTA) * NUMBER_OF_TETROMINOS - tetro_sum,
                 random = rand() % interval;

    while (random >= tetro_max + DELTA - tetro_count[tetro_id])
    {
        random -= (tetro_max + DELTA - tetro_count[tetro_id]);
        tetro_id++;
    }

    tetro_count[tetro_id]++;
    tetro_sum++;


    /// Fill our brand new tetro with values
    float color_r=0.0f;
    float color_g=0.0f;
    float color_b=0.0f;
    float color_a=0.0f;

    std::shared_ptr<Texture> texture_ptr;

    // MinGW cross 4.6 does not support list initializer assignments with c arrays...
    std::vector <unsigned char> shape;

    switch (tetro_id) {

    case 0:

        tetro_ptr->shape_size = 3;
        tetro_ptr->center_x = 1.0f;
        tetro_ptr->center_y = 1.5f;

        shape = {
            1, 0, 0,
            1, 1, 1,
            0, 0, 0
        };

        color_r = 0.8f;
        color_g = 0.2f;
        color_b = 0.2f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 1:

        tetro_ptr->shape_size = 3;
        tetro_ptr->center_x = 1.0f;
        tetro_ptr->center_y = 1.5f;

        shape = {
            0, 0, 1,
            1, 1, 1,
            0, 0, 0
        };

        color_r = 0.2f;
        color_g = 0.2f;
        color_b = 0.8f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 2:

        tetro_ptr->shape_size = 3;
        tetro_ptr->center_x = 1.0f;
        tetro_ptr->center_y = 1.5f;

        shape = {
            0, 1, 0,
            1, 1, 1,
            0, 0, 0
        };

        color_r = 0.2f;
        color_g = 0.8f;
        color_b = 0.2f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 3:

        tetro_ptr->shape_size = 2;
        tetro_ptr->center_x = 0.5f;
        tetro_ptr->center_y = 0.5f;

        shape = {
            1, 1,
            1, 1
        };

        color_r = 0.5f;
        color_g = 0.5f;
        color_b = 0.5f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 4:

        tetro_ptr->shape_size = 4;
        tetro_ptr->center_x = 1.5f;
        tetro_ptr->center_y = 2.0f;

        shape = {
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        color_r = 0.8f;
        color_g = 0.8f;
        color_b = 0.2f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 5:

        tetro_ptr->shape_size = 3;
        tetro_ptr->center_x = 1.0f;
        tetro_ptr->center_y = 1.5f;

        shape = {
            0, 1, 1,
            1, 1, 0,
            0, 0, 0
        };

        color_r = 0.8f;
        color_g = 0.2f;
        color_b = 0.8f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    case 6:

        tetro_ptr->shape_size = 3;
        tetro_ptr->center_x = 1.0f;
        tetro_ptr->center_y = 1.5f;

        shape = {
            1, 1, 0,
            0, 1, 1,
            0, 0, 0
        };

        color_r = 0.2f;
        color_g = 0.8f;
        color_b = 0.8f;
        color_a = 0.8f;

        texture_ptr = texmanager.get("crate3.bmp");

        break;

    }

    for (unsigned char y=0; y<tetro_ptr->shape_size; y++)
        for (unsigned char x=0; x<tetro_ptr->shape_size; x++)
            if (shape[x*tetro_ptr->shape_size+y] > 0)
                tetro_ptr->shape[x][y] = std::unique_ptr<Brick>(new Brick(
                    color_r, color_g, color_b, color_a, texture_ptr));
            else
                tetro_ptr->shape[x][y] = std::unique_ptr<Brick>(new Brick());


    return tetro_ptr;
}


void TetroFactory::reset()
{

    tetro_sum = 0;

    for (int i = 0; i < NUMBER_OF_TETROMINOS; i++) tetro_count[i] = 0;
}

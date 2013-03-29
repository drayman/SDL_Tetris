#ifndef TABLE_H
#define TABLE_H

#include "texture/texture_manager.h"
#include "mesh/cube_mesh.h"
#include "game/tetromino.h"
#include "game/brick.h"

/**
    This class represents the game board
*/

class Table
{

public:

    Table(TextureManager& texture_manager, CubeMesh& cube_mesh);

    void init();

    void reset();

    void draw(int time_spent, bool use_textures);

    bool tetroCanFit(Tetromino* tetro, int x, int y);

    void placeTetro(Tetromino* tetro, int x, int y);

    void removeTetro(Tetromino* tetro, int x, int y);

    unsigned int checkFullLines(int first_line, int last_line);

    bool updateFullLines();


private:

    void drawBrick(Brick *brick);

    TextureManager& texmanager;

    CubeMesh& cube;

    std::unique_ptr<Brick> table[12][21];

    std::vector<unsigned int> exploding_line;

    int timeSpent;

    bool useTextures;
};

#endif // TABLE_H


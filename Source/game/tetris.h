#ifndef TETRIS_H
#define TETRIS_H

#include "mesh/cube_mesh.h"
#include "game/tetromino.h"
#include "game/key_map.h"
#include "game/table.h"
#include "game/message_queue.h"
#include "texture/texture_manager.h"

#include <memory>
#include <vector>

class Tetris {

public:

    Tetris(TextureManager& texture_manager, Font& font);
    void init();
    void reset();
    void initTextures();
    bool update();
    void draw(bool draw_textures);
    void drawNext(bool draw_textures);

    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();

    unsigned int level;
    unsigned int lines;
    unsigned int score;
    unsigned int combo;

private:

    void rotateTetro();

    void createTetro();

    void createLeftAnim();
    void createRightAnim();
    void createDownAnim();
    void createRotateAnim();

    void createScoreMsg(int score);

    TextureManager& texmanager;
    Font& font;
    TetroFactory tetroFactory;
    Table table;

    MessageQueue messages;
    KeyMap keyMap;
    CubeMesh cube;

    float nextRot;
    float nextAccel;
    std::unique_ptr<Tetromino> nextTetro;
    std::unique_ptr<Tetromino> currTetro;

    int curr_x;
    int curr_y;

    unsigned char ticks_left;

    unsigned char stage;

};

#endif // TETRIS_H

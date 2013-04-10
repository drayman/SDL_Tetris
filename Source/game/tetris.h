#ifndef TETRIS_H
#define TETRIS_H

#include "mesh/cube_mesh.h"
#include "game/tetromino.h"
#include "keymap/keymap.h"
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
    void setKeyRepeat(bool repeat);
    void setTextureDrawing(bool textures);
    unsigned int getCurrentStage();
    bool update();
    void draw();
    void drawNext();


    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void releaseLeft();
    void releaseRight();
    void releaseRotate();

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

    bool draw_textures;

    unsigned int ticks_left;
    unsigned int stage;

};

#endif // TETRIS_H

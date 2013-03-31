#include "animation/translate_animation.h"
#include "animation/rotate_animation.h"
#include "animation/alpha_animation.h"
#include "game/animated_text.h"

#include <sstream>

#include "tetris.h"

Tetris::Tetris
(
    TextureManager& texture_manager,
    Font& character_font
):
    texmanager(texture_manager),
    font(character_font),
    tetroFactory(texture_manager),
    table(texture_manager, cube)
{

    cube.scale(0.9f,0.9f,0.9f);

}

void Tetris::init()
{
    table.init();
    reset();
}

void Tetris::reset()
{

    table.reset();

    keyMap.reset();

    nextRot = 0.0f;
    nextAccel = 2.0f;

    tetroFactory.reset();
    nextTetro = tetroFactory.createTetro(false);

    level = 0;
    lines = 0;
    score = 0;
    combo = 0;

    ticks_left = 30;
    stage = 0;

}


void Tetris::initTextures()
{
    texmanager.add("crate.bmp");
    texmanager.add("crate3.bmp");
}


void Tetris::moveLeft()
{                   // not like in c++11 enum class, KeyType is optional
    keyMap.setKey(KeyMap::LEFT);
}


void Tetris::moveRight()
{
    keyMap.setKey(KeyMap::RIGHT);
}


void Tetris::moveDown()
{
    /// TODO: if not in move
    keyMap.setKey(KeyMap::DOWN);
}


void Tetris::rotate()
{
    keyMap.setKey(KeyMap::ROTATE);
}


void Tetris::rotateTetro()
{

    int i,j,N;
    N=currTetro->shape_size;

    for(i=0; i<N; i++)
        for(j=i; j<N; j++)
            if(i!=j)
                currTetro->shape[i][j].swap(currTetro->shape[j][i]);


    for(i=0; i<N/2; i++)
        for(j=0; j<N; j++)
            currTetro->shape[j][i].swap(currTetro->shape[j][N-1-i]);
}


void Tetris::createTetro()
{
    currTetro = std::move(nextTetro);
    nextTetro = tetroFactory.createTetro(false);
    curr_x = 6-currTetro->shape_size/2;
    curr_y = 21-currTetro->shape_size;
}


void Tetris::createLeftAnim()
{
    for(unsigned char y=0; y < currTetro->shape_size; y++)
        for (unsigned char x=0; x < currTetro->shape_size; x++)
            if (currTetro->shape[y][x]->texture_ptr != NULL )
                currTetro->shape[y][x]->leftAnim=std::unique_ptr<Animation>(new TranslateAnimation(3, -1.0f, 0.0f));
}


void Tetris::createRightAnim()
{
    for(unsigned char y=0; y < currTetro->shape_size; y++)
        for (unsigned char x=0; x < currTetro->shape_size; x++)
            if (currTetro->shape[y][x]->texture_ptr != NULL )
                currTetro->shape[y][x]->rightAnim=std::unique_ptr<Animation>(new TranslateAnimation(3, 1.0f, 0.0f));
}


void Tetris::createDownAnim()
{
    for(unsigned char y=0; y < currTetro->shape_size; y++)
        for (unsigned char x=0; x < currTetro->shape_size; x++)
            if (currTetro->shape[y][x]->texture_ptr != NULL )
                currTetro->shape[y][x]->downAnim=std::unique_ptr<Animation>(new TranslateAnimation(3, 0.0f, -1.0f));
}


void Tetris::createRotateAnim()
{
    for(unsigned char y=0; y < currTetro->shape_size; y++)
        for (unsigned char x=0; x < currTetro->shape_size; x++)
            if (currTetro->shape[y][x]->texture_ptr != NULL )
                currTetro->shape[y][x]->rotateAnim=std::unique_ptr<Animation>(new RotateAnimation(6, -90.0f,
                    (float)(currTetro->shape_size-1)/2-y,
                    (float)(currTetro->shape_size-1)/2-x
                    ));
}


void Tetris::createScoreMsg(int score)
{
    AnimatedText* newText;

    std::string text;
    switch (combo) {
        case 3:  text = "Nice!"; break;
        case 5:  text = "Cool!"; break;
        case 8:  text = "Great!"; break;
        case 10: text = "Fantastic!"; break;
        case 12: text = "Awesome!"; break;
        case 15: text = "Amazing!"; break;
        case 20: text = "Incredible!"; break;
        case 25: text = "Unbelievable!"; break;
        case 30: text = "Fearsome..."; break;
        case 35: text = "Alexey Pajitnov"; break;
        default: break;
    }

    unsigned int delay = 0;
    if (text.length() > 0)
    {
        newText = new AnimatedText(font, text, 0.0f, 0.0f, -3.0f, delay, 0.5f, 0.65f,
                                                 Font::FontAlign::CENTER, Font::FontPlain::XY_3D);
        newText->attachAnimation(new TranslateAnimation(30, 0.0f, 0.0f, 10.0f));
        newText->attachAnimation(new AlphaAnimation(30, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f));
        messages.add(newText);

        delay = 30;
    }

    std::stringstream str_stream;
    float y_pos = 0.0f;
    if (combo > 1)
    {
        str_stream << combo << "x TETRIS";
        newText = new AnimatedText(font, str_stream.str(), 0.0f, 0.5f, -3.0f, delay, 0.5f, 0.65f,
                                                 Font::FontAlign::CENTER, Font::FontPlain::XY_3D);
        str_stream.str("");
        str_stream.clear();
        newText->attachAnimation(new TranslateAnimation(30, 0.0f, 0.0f, 10.0f));
        newText->attachAnimation(new AlphaAnimation(30, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f));
        messages.add(newText);

        y_pos = -0.5f;
    }

    str_stream << score;
    newText = new AnimatedText(font, str_stream.str(), 0.0f, y_pos,-3.0f, delay, 0.5f, 0.65f,
                                             Font::FontAlign::CENTER, Font::FontPlain::XY_3D);
    newText->attachAnimation(new TranslateAnimation(30, 0.0f, 0.0f, 10.0f));
    newText->attachAnimation(new AlphaAnimation(30, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f));
    messages.add(newText);

}


bool Tetris::update()
{

    if (stage==0)
    {
        createTetro();
        if (!table.tetroCanFit(currTetro.get(), curr_x, curr_y)) return false;  // GAME OVER
        stage = 1;
    }
    else if (stage == 1)
    {
        ticks_left--;


        if (keyMap.getKey(KeyMap::LEFT))
        {
            curr_x--;
            if (!table.tetroCanFit(currTetro.get(), curr_x, curr_y))
                curr_x++;
            else
                createLeftAnim();
        }


        if (keyMap.getKey(KeyMap::RIGHT))
        {
            curr_x++;
            if (!table.tetroCanFit(currTetro.get(), curr_x, curr_y))
                curr_x--;
            else
                createRightAnim();
        }

        if (keyMap.getKey(KeyMap::DOWN))
        {
            ticks_left = 0;
            score+=level+1;
        }

        if (keyMap.getKey(KeyMap::ROTATE))
        {
            rotateTetro();
            if (!table.tetroCanFit(currTetro.get(), curr_x, curr_y))
            {
                rotateTetro();
                rotateTetro();
                rotateTetro();
            }
            else
                createRotateAnim();
        }


        if (ticks_left == 0)
        {

            // set nonlinear levels
            switch (level) {
            case 0:
            case 1:
            case 2:
            case 3:
                ticks_left = 30-level*5;
                break;
            case 4:
            case 5:
                ticks_left = 20-level*2;
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                ticks_left = 14-level;
                break;
            default:
                break;
            }

            curr_y--;
            if (table.tetroCanFit(currTetro.get(), curr_x, curr_y))
                createDownAnim();
            else
            {
                curr_y++;
                table.placeTetro(currTetro.get(), curr_x, curr_y);

                // If there's fulll line  jump to the next stage. It would be nice to watch the explosion
                // of lines while we're playing but it's disadvantageous if there's only a few lines left
                int lines_exploded;
                if ((lines_exploded = table.checkFullLines(curr_y, curr_y+currTetro->shape_size)) > 0)
                {
                    // increase score, lines, level
                    int curr_score = 0;
                    lines+=lines_exploded;
                    switch (lines_exploded) {
                        case 1: curr_score=(level+1)*40;   break;
                        case 2: curr_score=(level+1)*100;  break;
                        case 3: curr_score=(level+1)*300;  break;
                        case 4: curr_score=(level+1)*1200; break;
                        default: break;
                    }
                    if (level<9) level = lines/10;  // Ohh, #define MAX_LEVEL 9;

                    if (lines_exploded==4) combo++; else combo = 0;

                    if (combo>1) curr_score+=curr_score*(combo-1)/2;

                    score+=curr_score;

                    // make a bubble appear
                    createScoreMsg(curr_score);

                    stage = 2;
                }
                else stage = 0;
            }
        }
    }
    else if (stage == 2 ) // remove the full lines
    {
        if (!table.updateFullLines())

            // get a new brick
            stage = 0;
    }

    keyMap.update();

    // Rotate the next tetro indicator
    if      (nextRot < 360 && nextRot >= 255) nextAccel-=0.1f;
    else if (nextRot < 255 && nextRot >= 185) nextAccel+=0.15f;
    else if (nextRot < 180 && nextRot >=  75) nextAccel-=0.1f;
    else if (nextRot <  75 && nextRot >=   5) nextAccel+=0.15f;
    if ((nextRot+=nextAccel) >= 360.f ) {
        nextRot=0.0f;
        nextAccel = 2.0f;
    }

    return true;
}


void Tetris::draw(bool draw_textures)
{
    if (stage == 1) table.placeTetro(currTetro.get(), curr_x, curr_y);
    table.draw(1, draw_textures);
    if (stage == 1) table.removeTetro(currTetro.get(), curr_x, curr_y);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glLoadIdentity();
    messages.draw(1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

}


void Tetris::drawNext(bool draw_textures)
{
    glClear( GL_DEPTH_BUFFER_BIT );
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);

    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef( -35, 0.0f, 1.0f, 0.0f );
    glRotatef(-nextRot, 0.0f, 0.0f, 1.0f );
    glTranslatef(-nextTetro->center_x, -nextTetro->center_y, 0.0f);

    cube.draw_init(true, false, draw_textures);

    for(unsigned char y=nextTetro->shape_size; y>0; y--) {
        for (unsigned char x=1; x<=nextTetro->shape_size; x++){
            if (nextTetro->shape[y-1][x-1]->texture_ptr!=NULL)
            {
                Brick* brick = nextTetro->shape[y-1][x-1].get();

                if (draw_textures) brick->texture_ptr->bind();

                glColor4f(brick->color_r, brick->color_g, brick->color_b, brick->color_a);
                cube.draw_only();
            }
            glTranslatef(1.0f, 0.0f, 0.0f);
        }
        glTranslatef( -nextTetro->shape_size, 1.0f, 0.0f);
    }
    cube.draw_close();
}

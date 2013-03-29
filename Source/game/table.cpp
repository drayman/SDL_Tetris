#include "animation/alpha_animation.h"
#include "animation/translate_animation.h"

#include "game/table.h"


Table::Table
(
    TextureManager& texture_manager,
    CubeMesh& cube_mesh
):
    texmanager(texture_manager),
    cube(cube_mesh)
{
    ;
}


void Table::init()
{
    //  GL use:       Arrays are:
    //
    //   ^               y
    // y |             +--->
    //   +--->       x |
    //     x           v
    //
    //  To keep the design simple, i replace x and y in drawing
    //  and process the models with inverted y axis...annoying


    TextureManager::Resource texture_ptr = texmanager.get("crate.bmp");

    for (int i=0;i<10;i++){
        table[0][i+1] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
        table[0][i+11] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
        table[11][i+1] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
        table[11][i+11] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
        table[i+1][0] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
    }
    table[0][0] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));
    table[11][0] = std::unique_ptr<Brick>(new Brick(0.9f, 0.9f, 0.9f, 1.0f, texture_ptr));

    reset();

}


void Table::reset()
{
    for(int y=1; y<21; y++)
        for (int x=1;x<11;x++)
            table[x][y].release();
}

bool Table::tetroCanFit(Tetromino* tetro, int x, int y)    // HELL AGAIN
{                             // nullptr isn`t supported on cross-mingw 4.6...WTF
    for(int j=0; j < (int)tetro->shape_size; j++)
        for (int i=0; i < (int)tetro->shape_size; i++)
            if (tetro->shape[j][i]->texture_ptr != NULL)
                if (table[x+i][y+tetro->shape_size-j-1] != NULL)
                    return false;

    // in addition: int. there was many problems using unsigned char
    // LOCAL_CFLAGS := -fsigned-char for Android. cus there char is unsigned char.
    // really better not to use, i'm not sure about side effects...madness

    return true;
}


void Table::placeTetro(Tetromino* tetro, int x, int y)
{
    for(int j=0; j < tetro->shape_size; j++)
        for (int i=0; i < tetro->shape_size; i++)
            if (tetro->shape[j][i]->texture_ptr != NULL )
                table[x+i][y+tetro->shape_size-j-1].swap(tetro->shape[j][i]);
}


void Table::removeTetro(Tetromino* tetro, int x, int y)
{
    for(int j=0; j < tetro->shape_size; j++)
        for (int i=0; i < tetro->shape_size; i++)
            if (tetro->shape[j][i] == NULL )
                tetro->shape[j][i] = std::move(table[x+i][y+tetro->shape_size-j-1]);
}


unsigned int Table::checkFullLines(int first_line, int last_line)
{
    bool line_found;
    int x,y;

    for (y=first_line; y<last_line; y++) {

        if (y<1 || y>20) continue;

        line_found = true;

        for (x=1; x<11; x++) {
            if (table[x][y] == NULL)
            {
                line_found = false;
                break;
            }
        }

        if (line_found )
        {
            // remember this line
            exploding_line.push_back(y);

            // make the line disappear
            for (x=1; x<11; x++)
                table[x][y]->alphaAnim=std::unique_ptr<Animation>(new AlphaAnimation(12,
                    table[x][y]->color_r, table[x][y]->color_g, table[x][y]->color_b,
                    table[x][y]->color_a, 0.0f));
        }
    }
    return exploding_line.size();
}


bool Table::updateFullLines()
{
    // check if animation is still in process
    if (!table[1][exploding_line[0]]->downAnim->isActive() &&
        !table[1][exploding_line[0]]->alphaAnim->isActive())
    {
        unsigned int x, y, c;
        unsigned int lines_exploded = exploding_line.size();

        // get rid of those lines
        for (x=1; x<11; x++)
            for (y=0; y<lines_exploded; y++)
                table[x][exploding_line[y]].release();

        // make them fall
        c = 1;                                                 // this gonna show how much the next line need to fall
        for (y=exploding_line[0]+1; y<21; y++)                 // iterate from the first explosive line
            if (c < lines_exploded && y==exploding_line[c])
                c++;
            else
                for (x=1; x<11; x++)
                    if (table[x][y] != NULL)
                    {
                        table[x][y]->downAnim =
                        std::unique_ptr<Animation>(new TranslateAnimation(c*3, 0.0f, c*-1.0f));

                        table[x][y-c] = std::move(table[x][y]);
                    }

        exploding_line.clear();

        return false;
    }

    return true;
}

void Table::draw(int time_spent, bool use_textures)
{
    useTextures = use_textures;
    timeSpent = time_spent;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    cube.draw_init(true, false, use_textures);

    glTranslatef(-5.5f, -10.0f, -11.5f);
    glPushMatrix();

    int x,y;

    for(y=0; y<11; y++) {
        for (x=0; x<6; x++){
            if (table[x][y]!=NULL) drawBrick(table[x][y].get());
            glTranslatef(1.0f, 0.0f, 0.0f);
        }
        glTranslatef(-6.0f, 1.0f, 0.0f);
    }

    glTranslatef(11.0f, -11.0f, 0.0f);
    for(y=0; y<11; y++) {
        for (x=11; x>5; x--){
            if (table[x][y]!=NULL) drawBrick(table[x][y].get());
            glTranslatef(-1.0f, 0.0f, 0.0f);
        }
        glTranslatef(6.0f, 1.0f, 0.0f);
    }

    glTranslatef(-11.0f, 9.0f, 0.0f);
    for(y=20; y>10; y--) {
        for (x=0; x<6; x++){
            if (table[x][y]!=NULL) drawBrick(table[x][y].get());
            glTranslatef(1.0f, 0.0f, 0.0f);
        }
        glTranslatef(-6.0f, -1.0f, 0.0f);
    }

    glTranslatef(11.0f, 10.0f, 0.0f);
    for(y=20; y>10; y--) {
        for (x=11; x>5; x--){
            if (table[x][y]!=NULL) drawBrick(table[x][y].get());
            glTranslatef(-1.0f, 0.0f, 0.0f);
        }
        glTranslatef(6.0f, -1.0f, 0.0f);
    }

    glPopMatrix();

    cube.draw_close();
    glDisable(GL_BLEND);
}

void Table::drawBrick(Brick* brick)
{
    glColor4f(brick->color_r, brick->color_g, brick->color_b, brick->color_a);

    if (useTextures && brick->texture_ptr!=NULL )
        brick->texture_ptr->bind();

    glPushMatrix();

    if (brick->leftAnim != NULL && brick->leftAnim->isActive())
        brick->leftAnim->transform(timeSpent);

    if (brick->rightAnim != NULL && brick->rightAnim->isActive())
        brick->rightAnim->transform(timeSpent);

    if (brick->downAnim != NULL && brick->downAnim->isActive())
        brick->downAnim->transform(timeSpent);

    if (brick->rotateAnim != NULL && brick->rotateAnim->isActive())
        brick->rotateAnim->transform(timeSpent);

    if (brick->alphaAnim != NULL && brick->alphaAnim->isActive())
        brick->alphaAnim->transform(timeSpent);

    cube.draw_only();

    glPopMatrix();
}

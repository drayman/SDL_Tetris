#include "log/log.h"
#include "font.h"

#include <vector>

void Font::set_texture( const std::shared_ptr<Texture> texture,
           const GLuint m_width, const GLuint m_height,
           const GLuint c_width, const GLuint c_height,
           const char first_letter )
{
    this->texture = texture;
    this->m_width = m_width; this->m_height = m_height;
    this->c_width = c_width; this->c_height = c_height;
    this->first_letter = first_letter;

    c_per_row = m_width/c_width;

    //calculate how wide each character is in term of texture coords
    dtx = float(c_width)/float(m_width);
    dty = float(c_height)/float(m_height);

    glEnable(GL_TEXTURE_2D);
    texture->bind();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glDisable(GL_TEXTURE_2D);
}


void Font::set_align(FontAlign font_alignment) {
    if (font_alignment == FontAlign::UNDEFINED) return;
    align = font_alignment;
}


void Font::set_plain(FontPlain font_plain) {
    if (font_plain == FontPlain::UNDEFINED) return;
    plain = font_plain;
}


void Font::set_size(GLfloat c_w, GLfloat c_h) {
    if ( c_w > 0.0f ) cw = c_w;
    if ( c_h > 0.0f ) ch = c_h;
}


void Font::draw_text(
    GLfloat x, GLfloat y, GLfloat z,
    const char* text,
    GLfloat c_w, GLfloat c_h,
    GLfloat angle,
    FontAlign font_alignment,
    FontPlain font_direction)
{
    if (c_per_row==0) {
        LOGW("Uninitialized Font used");
        return;
    }

    if ( !texture->isInitialized() ) {
        LOGW("Font with broken texture used");
        return;
    }

    if (c_w <= 0.0f) c_w = cw;
    if (c_h <= 0.0f) c_h = ch;

    if (font_direction==FontPlain::UNDEFINED) {
        font_direction = plain;
    }

    if (font_alignment==FontAlign::UNDEFINED) {
        font_alignment = align;
    }

     //character location and dimensions
    GLfloat c_x = 0.0f;
    GLfloat c_y = 0.0f;

    // center the text if needed
    GLfloat offset_h = 0, offset_w = 0;
    if ( font_alignment == FontAlign::CENTER ) {
        offset_h = c_h / 2.0f;
        offset_w = (float)strlen(text) * c_w / 2.0f;
    }
    else if ( font_alignment == FontAlign::RIGHT ) {
        offset_w = (float)strlen(text) * c_w;
    }

    // bind texture
    glEnable(GL_TEXTURE_2D);
    texture->bind();

    // set transparent background
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);

    glPushMatrix();

    // move it
    glTranslatef(x, y, z);

    // rotate it
    if (angle != 0.0f) switch (font_direction) {
        case FontPlain::UNDEFINED:  // UNDEFINED can not happen, anyway
        case FontPlain::XY_2D:
        case FontPlain::XY_3D:
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            break;
        case FontPlain::XZ_3D:
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            break;
        case FontPlain::YZ_3D:
            glRotatef(angle, 1.0f, 0.0f, 0.0f);
            break;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (const char* c = text; *c != 0; c++, c_x += c_w) {
        // subtract the value of the first char in the character map
        // to get the index in our map
        int index = ((upletters)?toupper(*c):(*c)) - first_letter;

        if ( index < 0 && index+first_letter != ' ' ) {
            LOGW("Character outside of font");
            continue;
        }

        // get the indexes
        int row = index/c_per_row;
        int col = index%c_per_row;

        // find the texture coords
        GLfloat tx = float(col * c_width)/float(m_width);
        GLfloat ty = float(row * c_height)/float(m_height);

        // Problem 1:
        // Vertices can't be declared inside in a switch without introducing ugly scope brackets
        // No problem, it's time for lovely compound literals...
        // We could solve it with vector, or c++0x initializer list as well
        // Problem 2
        // after upgrading to mingw gcc 4.8
        // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53220
        // on gcc 4.6 still getting error: assigning to an array from an initializer list,
        // however it should work: http://gcc.gnu.org/gcc-4.6/cxx0x_status.html
        // So, vectorize...

        std::vector<GLfloat> Vertices;

        switch (font_direction) {
            case FontPlain::XY_2D:
            case FontPlain::UNDEFINED:
                Vertices = {
                    c_x-offset_w,     c_y-offset_h+c_h,
                    c_x-offset_w,     c_y-offset_h,
                    c_x-offset_w+c_w, c_y-offset_h,
                    c_x-offset_w+c_w, c_y-offset_h+c_h
                };
                glVertexPointer(2, GL_FLOAT, 0, &Vertices[0]);
                break;

            case FontPlain::XY_3D:
                Vertices = {
                    c_x-offset_w,     c_y-offset_h+c_h, 0.0f,
                    c_x-offset_w,     c_y-offset_h,     0.0f,
                    c_x-offset_w+c_w, c_y-offset_h,     0.0f,
                    c_x-offset_w+c_w, c_y-offset_h+c_h, 0.0f
                };
                glVertexPointer(3, GL_FLOAT, 0, &Vertices[0]);
                break;

            case FontPlain::XZ_3D:
                Vertices = {
                    c_x-offset_w,     0.0f,  c_y-offset_h+c_h,
                    c_x-offset_w,     0.0f,  c_y-offset_h,
                    c_x-offset_w+c_w, 0.0f,  c_y-offset_h,
                    c_x-offset_w+c_w, 0.0f,  c_y-offset_h+c_h
                };
                glVertexPointer(3, GL_FLOAT, 0, &Vertices[0]);
                break;

            case FontPlain::YZ_3D:
                Vertices = {
                    0.0f, c_x-offset_w,     c_y-offset_h+c_h,
                    0.0f, c_x-offset_w,     c_y-offset_h,
                    0.0f, c_x-offset_w+c_w, c_y-offset_h,
                    0.0f, c_x-offset_w+c_w, c_y-offset_h+c_h
                };
                glVertexPointer(3, GL_FLOAT, 0, &Vertices[0]);
                break;
        }

        GLfloat Texices[] = {
            tx,     ty,
            tx,     ty+dty,
            tx+dtx, ty+dty,
            tx+dtx, ty
        };

        glTexCoordPointer(2, GL_FLOAT, 0, Texices);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
}

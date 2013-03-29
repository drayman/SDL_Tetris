#include "log/log.h"
#include "font.h"

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
    align = font_alignment;
}


void Font::set_size(GLfloat c_w, GLfloat c_h) {
    cw = c_w; ch = c_h;
}


void Font::draw_text( GLfloat x, GLfloat y, GLfloat z,
                      const char* text,
                      FontPlain direction,
                      GLfloat angle )
{
    if (c_per_row==0) {
        LOGW("Warning: uninitalized font is used");
        return;
    }

     //character location and dimensions
    GLfloat cx = 0.0f;
    GLfloat cy = 0.0f;

    // center the text if needed
    GLfloat offset_h = 0, offset_w = 0;
    if ( align == CENTER ) {
        offset_h = ch / 2.0f;
        offset_w = (float)strlen(text) * cw / 2.0f;
    }
    else if ( align == RIGHT ) {
        offset_w = (float)strlen(text) * cw;
    }

    // bind texture
    glEnable(GL_TEXTURE_2D);
    texture->bind();

    // set transparent background
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);

    // rotate it
    glPushMatrix();
    glTranslatef(x, y, z);
    switch (direction) {
        case XY_2D:
        case XY_3D:
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            break;
        case XZ_3D:
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            break;
        case YZ_3D:
            glRotatef(angle, 1.0f, 0.0f, 0.0f);
            break;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (const char* c = text; *c != 0; c++, cx += cw) {
        // subtract the value of the first char in the character map
        // to get the index in our map
        int index = ((upletters)?toupper(*c):(*c)) - first_letter; // toupper, cus i was lazy to draw all the letters...

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

        GLfloat *Vertices; // Can't be declared inside in a switch. No problem, it's time for lovely compund literals...

        switch (direction) {
            case XY_2D:
                Vertices = (GLfloat[]){
                    cx-offset_w,    cy-offset_h+ch,
                    cx-offset_w,    cy-offset_h,
                    cx-offset_w+cw, cy-offset_h,
                    cx-offset_w+cw, cy-offset_h+ch
                };
                glVertexPointer(2, GL_FLOAT, 0, Vertices);
                break;

            case XY_3D:
                Vertices = (GLfloat[]){
                    cx-offset_w,    cy-offset_h,    0.0f,
                    cx-offset_w,    cy-offset_h+ch, 0.0f,
                    cx-offset_w+cw, cy-offset_h+ch, 0.0f,
                    cx-offset_w+cw, cy-offset_h,    0.0f
                };
                glVertexPointer(3, GL_FLOAT, 0, Vertices);
                break;

            case XZ_3D:
                Vertices = (GLfloat[]){
                    cx-offset_w,    0.0f, cy-offset_h,
                    cx-offset_w,    0.0f, cy-offset_h+ch,
                    cx-offset_w+cw, 0.0f, cy-offset_h+ch,
                    cx-offset_w+cw, 0.0f, cy-offset_h
                };
                glVertexPointer(3, GL_FLOAT, 0, Vertices);
                break;

            case YZ_3D:
                Vertices = (GLfloat[]){
                    0.0f, cx-offset_w,    cy-offset_h,
                    0.0f, cx-offset_w,    cy-offset_h+ch,
                    0.0f, cx-offset_w+cw, cy-offset_h+ch,
                    0.0f, cx-offset_w+cw, cy-offset_h
                };
                glVertexPointer(3, GL_FLOAT, 0, Vertices);
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


void Font::draw_text( GLfloat x, GLfloat y, GLfloat z,
                const char* text,
                GLfloat c_w, GLfloat c_h,
                FontAlign font_align,
                FontPlain direction,
                GLfloat angle ) {

    FontAlign ex_align=align;           // save previous settings
    GLfloat ex_cw = cw, ex_ch= ch;

    align = font_align;                 // use the parameter settings
    cw = c_w; ch = c_h;

    draw_text( x, y, z, text, direction, angle );  // draw

    align = ex_align;                   // restore previous settings
    cw = ex_cw; ch = ex_ch;

}

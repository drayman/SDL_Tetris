#ifndef FONT_H
#define FONT_H

#include <memory>

#include "texture_manager.h"

class Font {

public:

    Font() : align(LEFT), cw(1), ch(1), c_per_row(0), upletters(false) {}

    /// Possible target plains
    enum FontPlain { XY_2D, XY_3D, XZ_3D, YZ_3D };

    /// Font alignments
    enum FontAlign { LEFT, CENTER, RIGHT };

    /// Assign a texture what holds the font characters
    void set_texture( const std::shared_ptr<Texture> texture,
         const GLuint m_width, const GLuint m_height,
         const GLuint c_width, const GLuint c_height,
         const char first_letter );

    /// Sets the alignment
    void set_align( FontAlign font_alignment );

    /// Sets the dimensions of one character in pixels
    void set_size( GLfloat c_w, GLfloat c_h );

    /// Set it to use uppercase or when the bitmap supports only uppercase letters
    void set_upper_font(GLboolean uppercase) {upletters=uppercase; }

    /** Draws the "text" to X,Y,Z.
        Direction defines the two used axis to draw the text.
        If XY_2D is used the position value of Z is omitted.
        Angle (0-360) defines the rotation on the selected plain.
        Size and alignment are used from previous set_align() and set_size(). */
    void draw_text( GLfloat x, GLfloat y, GLfloat z,
                    const char* text,
                    FontPlain direction = XY_2D,
                    GLfloat angle = 0 );

    /** Draws the "text" to X,Y,Z with c_w and c_h dimensions/char,
        using the value of font_align to set the text alignment.
        Direction defines the two used axis to draw the text.
        If XY_2D is used the position value of Z is omitted.
        Angle (0-360) defines the rotation on the selected plain. */
    void draw_text( GLfloat x, GLfloat y, GLfloat z,
                    const char* text,
                    GLfloat c_w, GLfloat c_h,
                    FontAlign font_align,
                    FontPlain direction = XY_2D,
                    GLfloat angle = 0 );

private:

    std::shared_ptr<Texture> texture;  ///< pointer to our bitmap

    FontAlign align;                   ///< alignment
    GLfloat  cw,  ch;                  ///< character display size
    GLfloat dtx, dty;                  ///< character texcoord size

    GLuint c_per_row;                  ///< char per row

    bool upletters;                    ///< for uppercase

    GLuint m_width, m_height;          ///< texture properties
    GLuint c_width, c_height;          ///< character properties
    char first_letter;                 ///< the first letter/sign in the bitmap
};

#endif // FONT_H

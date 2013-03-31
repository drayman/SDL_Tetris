#ifndef FONT_H
#define FONT_H

#include <memory>

#include "texture_manager.h"

class Font {

public:

    /// Possible target plains
    enum class FontPlain { UNDEFINED, XY_2D, XY_3D, XZ_3D, YZ_3D };

    /// Font alignments
    enum class FontAlign { UNDEFINED, LEFT, CENTER, RIGHT };


    /// Constructor
    Font() :
        align(FontAlign::LEFT),
        plain(FontPlain::XY_2D),
        cw(1.0f), ch(1.0f),
        c_per_row(0),
        upletters(false)
    {}

    /// Assign a texture what holds the font characters
    void set_texture( const std::shared_ptr<Texture> texture,
         const GLuint m_width, const GLuint m_height,
         const GLuint c_width, const GLuint c_height,
         const char first_letter );

    /// Sets the alignment
    void set_align( FontAlign font_alignment );

    /// Sets the alignment
    void set_plain( FontPlain font_plain );

    /// Sets the dimensions of one character in pixels
    void set_size( GLfloat c_w, GLfloat c_h );

    /// Set it to use uppercase or when the bitmap supports only uppercase letters
    void set_upper_font(GLboolean uppercase) { upletters = uppercase; }

    /** Draws the "text" to X,Y,Z with c_w and c_h dimensions/char.
        Angle (0-360) defines the rotation on the selected plain.
        The value of font_align to set the text alignment.
        Direction defines the two used axis to draw the text.
        If XY_2D is used the position value of Z is omitted. */
    void draw_text(
        GLfloat x, GLfloat y, GLfloat z,
        const char* text,
        GLfloat c_w = 0.0f, GLfloat c_h=0.0f,
        GLfloat angle = 0.0f,
        FontAlign font_alignment = FontAlign::UNDEFINED,
        FontPlain font_direction = FontPlain::UNDEFINED
    );

private:

    std::shared_ptr<Texture> texture;  ///< pointer to our bitmap

    FontAlign align;                   ///< alignment
    FontPlain plain;                   ///< the two axis used to draw

    GLfloat  cw,  ch;                  ///< character display size
    GLfloat dtx, dty;                  ///< character texcoord size

    GLuint c_per_row;                  ///< char per row

    bool upletters;                    ///< for uppercase

    GLuint m_width, m_height;          ///< texture properties
    GLuint c_width, c_height;          ///< character properties
    char first_letter;                 ///< the first letter/sign in the bitmap

};

#endif // FONT_H

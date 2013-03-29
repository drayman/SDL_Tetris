#ifndef ANIMATED_TEXT_H
#define ANIMATED_TEXT_H

#include "animation/animation.h"
#include "texture/font.h"

#include <string>
#include <vector>

class AnimatedText
{

public:

    AnimatedText(
        Font& font,
        std::string text,
        float pos_x,
        float pos_y,
        float pos_z = 0.0f,
        float char_w = 0.0f,
        float char_h = 0.0f,
        Font::FontPlain direction = Font::FontPlain::XY_2D
    );

    ~AnimatedText();

    void draw(int timeSpent);

    void attachAnimation(Animation* animation);

    bool isActive();

private:

    Font& _font;
    std::string _text;

    std::vector<Animation*> animations;

    Font::FontPlain direction;
    float x, y, z;
    float c_w, c_h;

};

#endif // TEXTBOX_H

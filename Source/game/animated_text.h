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
        unsigned int start_delay = 0.0f,
        float char_w = 0.0f,
        float char_h = 0.0f,
        Font::FontAlign font_alignment = Font::FontAlign::UNDEFINED,
        Font::FontPlain font_direction = Font::FontPlain::UNDEFINED
    );

    ~AnimatedText();

    void draw(unsigned int timeSpent);

    void attachAnimation(Animation* animation);

    bool isActive();

private:

    Font& _font;
    std::string _text;

    std::vector<Animation*> animations;

    float x, y, z;
    unsigned int delay;

    float c_w, c_h;
    Font::FontAlign align;
    Font::FontPlain direction;

};

#endif // TEXTBOX_H

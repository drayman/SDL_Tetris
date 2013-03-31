#include "platform.h"
#include "game/animated_text.h"

AnimatedText::AnimatedText(
    Font& font,
    std::string text,
    float pos_x,
    float pos_y,
    float pos_z,
    unsigned int start_delay,
    float char_w,
    float char_h,
    Font::FontAlign font_alignment,
    Font::FontPlain font_direction
) :
    _font(font)
{
    _text = text;
    x = pos_x;
    y = pos_y;
    z = pos_z;
    delay = start_delay;
    c_w = char_w;
    c_h = char_h;
    align = font_alignment,
    direction = font_direction;
}


void AnimatedText::draw(unsigned int time_spent)
{
    if (time_spent <= delay ) {
        delay-=time_spent;
    } else {
        time_spent -= delay;
        delay = 0;

        glPushMatrix();

        for (auto it = animations.begin(); it!=animations.end(); it++)
            (*it)->transform(time_spent);

        _font.draw_text(
            x, y, z, _text.c_str(),
            c_w, c_h,
            0.0f,
            align,
            direction
        );

        glPopMatrix();
    }
}


void AnimatedText::attachAnimation(Animation* animation)
{
    animations.push_back(animation);
}


bool AnimatedText::isActive()
{
    for (auto it = animations.begin(); it!=animations.end(); it++)
        if ((*it)->isActive()) return true;

    return false;
}

AnimatedText::~AnimatedText()
{
    for (auto it = animations.begin(); it!=animations.end(); it++)
        delete *it;
}

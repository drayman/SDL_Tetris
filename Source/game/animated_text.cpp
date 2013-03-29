#include "platform.h"
#include "game/animated_text.h"

AnimatedText::AnimatedText(
    Font& font,
    std::string text,
    float pos_x,
    float pos_y,
    float pos_z,
    float char_w,
    float char_h,
    Font::FontPlain direction
) :
    _font(font)
{
    _text = text;
    x = pos_x;
    y = pos_y;
    z = pos_z;
    c_w = char_w;
    c_h = char_h;
}

void AnimatedText::draw(int time_spent)
{
    if (c_w > 0.0f && c_h > 0.0f) _font.set_size(c_w, c_h);

    glPushMatrix();

    for (auto it = animations.begin(); it!=animations.end(); it++)
        (*it)->transform(time_spent);

    _font.draw_text(x, y, z, _text.c_str());

    glPopMatrix();
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

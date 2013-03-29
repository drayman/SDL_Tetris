#include "platform.h"

#include "animation/translate_animation.h"

TranslateAnimation::TranslateAnimation(
        float move_duration,
        float move_x,
        float move_y,
        float move_z,
        RepeatType repeat_type) : Animation(move_duration, repeat_type)
{
    x = move_x;
    y = move_y;
    z = move_z;
}

void TranslateAnimation::animate(float rate)
{
    rate = 1-rate;

    glTranslatef(
        -x*rate,
        -y*rate,
        -z*rate
    );
}

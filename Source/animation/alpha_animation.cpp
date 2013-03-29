#include "platform.h"

#include "animation/alpha_animation.h"

AlphaAnimation::AlphaAnimation(
    float alpha_duration,
    float color_r,
    float color_g,
    float color_b,
    float color_a_src,
    float color_a_dst,
    RepeatType repeat_type) : Animation(alpha_duration, repeat_type)
{
    r = color_r;
    g = color_g;
    b = color_b;
    a_src = color_a_src;
    a_diff = color_a_src - color_a_dst;
}

void AlphaAnimation::animate(float rate)
{
    glColor4f(
        r,
        g,
        b,
        a_src-rate*a_diff
    );
}

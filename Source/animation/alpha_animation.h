#ifndef ALPHA_ANIMATION_H
#define ALPHA_ANIMATION_H

#include "animation/animation.h"

class AlphaAnimation : public Animation
{

public:

    AlphaAnimation(
        float alpha_duration,
        float color_r,
        float color_g,
        float color_b,
	float color_a_src,
	float color_a_dst,
	RepeatType repeat_type = RepeatType::NONE);

protected:

    virtual void animate(float rate);

    float r;
    float g;
    float b;
    float a_src;
    float a_diff;
};

#endif // ALPHA_ANIMATION_H

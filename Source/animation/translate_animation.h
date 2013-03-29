#ifndef TRANSLATE_ANIMATION_H
#define TRANSLATE_ANIMATION_H

#include "animation/animation.h"

class TranslateAnimation : public Animation
{

public:

    TranslateAnimation(
        float move_duration,
        float move_x,
        float move_y,
        float move_z = 0.0f,
        RepeatType repeat_type = RepeatType::NONE);

protected:

    virtual void animate(float rate);

    float x,y,z;
};

#endif // TRANSLATE_ANIMATION_H

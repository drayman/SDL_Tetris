#ifndef ROTATE_ANIMATION_H
#define ROTATE_ANIMATION_H

#include "animation/animation.h"

class RotateAnimation : public Animation
{

public:

    enum class AxisType {X, Y, Z};

    RotateAnimation(
        float rotate_duration,
        float rotate_angle,
        float center_x = 0.0f,
        float center_y = 0.0f,
        float center_z = 0.0f,
        AxisType rotate_axis = AxisType::Z,
        RepeatType repeat_type = RepeatType::NONE);

protected:

    virtual void animate(float rate);

    float angle;
    AxisType axis;
    float x,y,z;
};

#endif // ROTATE_ANIMATION_H

#include "platform.h"

#include "animation/rotate_animation.h"

RotateAnimation::RotateAnimation(
    float rotate_duration,
    float rotate_angle,
    float center_x,
    float center_y,
    float center_z,
    AxisType rotate_axis,
    RepeatType repeat_type) : Animation(rotate_duration, repeat_type)
{
    angle = rotate_angle;
    axis = rotate_axis;
    x = center_x;
    y = center_y;
    z = center_z;
}

void RotateAnimation::animate(float rate)
{
    glTranslatef(y,-x,z);

    glRotatef( angle*(rate),
        (axis==AxisType::X)? 1.0f : 0.0f,
        (axis==AxisType::Y)? 1.0f : 0.0f,
        (axis==AxisType::Z)? 1.0f : 0.0f
    );

    glTranslatef(-x,-y,-z);

}

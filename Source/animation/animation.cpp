#include "animation/animation.h"

Animation::Animation(int anim_duration, RepeatType repeat_type )
{
    if (anim_duration<=0) anim_duration = 1;
    duration = timer = anim_duration;
    repeat = repeat_type;
    direction = 1.0f;
}

float Animation::transform(int time_spent)
{
    float time_overflow = 0;

    timer-=time_spent*direction;

    if (repeat==RepeatType::NONE)
    {
        time_overflow = timer;

        if (timer<0)
            timer = 0;
        else if (timer>duration)
            timer = duration;
    }
    else if (repeat == RepeatType::ONE_WAY)
    {
        while (timer < 0) timer += duration;
        while (timer > duration) timer -= duration;
    }
    else if (repeat == RepeatType::TWO_WAY)
    {       // math.h fmod would be constant time, but it's awkward on different platforms
        while (timer < -duration) timer += duration*2;
        while (timer > duration*2) timer -= duration*2;

        if (timer < 0)
        {
            timer *= -1.0f;
            direction = - direction;
        }
        else if (timer > duration)
        {
            timer = duration*2 - timer;
            direction = -direction;
        }
    }

    animate(1.0f - (float)timer/duration);

    return time_overflow;
}

bool Animation::isActive() {
    return (repeat==RepeatType::NONE)
        ? (timer > 0 && timer <= duration )
        : true
    ;
}

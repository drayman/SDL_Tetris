#ifndef ANIMATION_H
#define ANIMATION_H

/**
    Animation base class

    Provides a common interface for animations
    Any child class must call this base class constructor manually in order to set up
    the animation properly
    To implement an animation, override the "animate()" method. This method is called by
    transform(), and takes a parameter value between [0..1] where 0 is the starting state
*/

class Animation
{

public:

    /// Represents the animations repeat type.
    /// NONE is a simple animation that runs once,
    /// ONE_WAY starts in the beginning phase when finished,
    /// TWO_WAY is animated in both direction
    enum class RepeatType { NONE, ONE_WAY, TWO_WAY };

    /// Base constructor, with animation duration/interval and repeat type
    Animation(int anim_duration, RepeatType repeat_type = RepeatType::NONE);

    /// Public method to update the animation's state
    /// An animation can be rewinded with negative parameter
    float transform(int time_spent);

    /// If operates in RepeatType::NONE, shows if the animation reached
    /// it's final state. In other repeat modes it always returns true
    bool isActive();

    // i really hate to create getters and setters
    int duration;           ///< Represents the interval
    int timer;              ///< Time spent/remaining between intervals
    RepeatType repeat;      ///< Type of repeat

protected:

    virtual void animate(float rate) = 0;    ///< Animation-specific implementation
    float direction;                         ///< Internal flag for actual direction

};


#endif // ANIMATION_H

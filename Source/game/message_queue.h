#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>

#include "game/animated_text.h"

class MessageQueue
{

public:

     MessageQueue();

     ~MessageQueue();

    void add(AnimatedText* text);

    void draw(int time_spent);

    void clear();

private:
    std::queue<AnimatedText*> messages;

};

#endif // MESSAGE_QUEUE_H

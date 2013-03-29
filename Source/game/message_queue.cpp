#include "message_queue.h"

MessageQueue::MessageQueue()
{

}


MessageQueue::~MessageQueue()
{
    while (!messages.empty())
    {
        delete messages.front();
        messages.pop();
    }
}


void MessageQueue::add(AnimatedText* text)
{
    messages.push(text);
}


void MessageQueue::draw(int time_spent)
{
    for (unsigned int i = 0; i < messages.size(); i++)
    {
        AnimatedText* msg = messages.front();
        messages.pop();

        msg->draw(time_spent);

        if (msg->isActive())
            messages.push(msg);
        else
            delete msg;
    }
}

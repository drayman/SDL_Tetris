#include "keymap.h"

KeyMap::KeyMap()
{
}

KeyMap::~KeyMap()
{
    auto it = key_store.begin();
    while (it !=key_store.end())
        delete (it++)->second;
}

void KeyMap::registerKey(KeyType key, unsigned int repeat_interval)
{
    key_store[static_cast<int>(key)] = new KeyPress(repeat_interval);
}

void KeyMap::update(unsigned int time_spent)
{
    auto it = key_store.begin();
    while (it != key_store.end())
    {
        auto keypress = (it++)->second;
        if (keypress->curr_time > 0)
        {
            if (time_spent > keypress->curr_time)
                keypress->curr_time = 0;
            else
                keypress->curr_time -= time_spent;
        }
    }
}

void KeyMap::pressKey(KeyType key)
{
    auto keypress = key_store[static_cast<int>(key)];
    if ((repeat_mode && keypress->curr_time==0) ||
        (!repeat_mode && keypress->released))
    {
        keypress->curr_time = keypress->interval;
        keypress->pressed++;
        keypress->released = false;
    }
}


void KeyMap::releaseKey(KeyType key)
{
    key_store[static_cast<int>(key)]->released = true;
    key_store[static_cast<int>(key)]->curr_time = 0;
}

unsigned int KeyMap::getKey(KeyType key)
{
    unsigned int temp = key_store[static_cast<int>(key)]->pressed;
    key_store[static_cast<int>(key)]->pressed = 0;
    return temp;
}

void KeyMap::clear()
{
    auto it = key_store.begin();
    while (it != key_store.end())
    {
        it->second->curr_time = 0;
        it->second->pressed = 0;
        it++;
    }
}

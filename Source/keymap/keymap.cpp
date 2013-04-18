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
    int key_id = static_cast<int>(key);
    if (key_store[key_id] != nullptr ) delete key_store[key_id];
    key_store[key_id] = new KeyPress(repeat_interval);
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
    int key_id = static_cast<int>(key);
    key_store[key_id]->released = true;
    key_store[key_id]->curr_time = 0;
}

unsigned int KeyMap::getKey(KeyType key)
{
    int key_id = static_cast<int>(key);
    unsigned int temp = key_store[key_id]->pressed;
    key_store[key_id]->pressed = 0;
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

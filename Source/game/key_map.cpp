#include "key_map.h"

KeyMap::KeyMap()
{
    interval=1;
    key_interval[LEFT]=3;
    key_interval[RIGHT]=3;
    key_interval[DOWN]=3;
    key_interval[ROTATE]=6;
    reset();
}

bool KeyMap::getKey(KeyType key)
{
    if (key_delay[key] == interval)
        return true;
    return false;
}

void KeyMap::setKey(KeyType key)
{
    if (key_delay[key] == 0)
        key_delay[key] = key_interval[key]*interval;
}

void KeyMap::clearKey(KeyType key)
{
    key_delay[key] = 0;
}


void KeyMap::update()
{
    for (int i=0; i<4; i++) {
        if (key_delay[i]>0) key_delay[i]--;
    }
}

void KeyMap::reset()
{
    for (int i=0; i<4; i++) {
        key_delay[i]=0;
    }
}

void KeyMap::setBaseInterval(unsigned char repeat_interval)
{
    interval = repeat_interval;
}

void KeyMap::setInterval(KeyType key, unsigned char repeat_interval)
{
    key_interval[key] = repeat_interval;
}

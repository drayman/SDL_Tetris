#ifndef KEY_MAP_H
#define KEY_MAP_H

class KeyMap {

public:

    enum KeyType { LEFT, RIGHT, DOWN, ROTATE };

    KeyMap();

    bool getKey(KeyType key);

    void setKey(KeyType key);

    void clearKey(KeyType key);

    void update();

    void reset();

    void setBaseInterval(unsigned char repeat_interval);

    void setInterval(KeyType key, unsigned char repeat_interval);

private:

    unsigned char key_delay[4];
    unsigned char key_repeat[4];
    unsigned char key_interval[4];
    unsigned char interval;
    bool repeat_mode;
};

#endif // KEY_MAP_H

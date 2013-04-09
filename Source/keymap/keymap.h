#include "key_values.h"

#include <map>

class KeyMap
{

public:

    /// Constructor
    KeyMap();

    /// Destructor
    ~KeyMap();

    /// Function for registering a key we want to use
    /// @param key The id of the key
    /// @param repeat_interval The keypress calmdown time
    /// @see KeyType
    void registerKey(KeyType key, unsigned int repeat_interval);

    /// Calculates the keypresses
    /// @param time_spent Time spent since the last update
    void update(unsigned int time_spent);

    /// Call this "presses the button"
    /// @param key The id of the key
    /// @see KeyType
    void pressKey(KeyType key);

    /// Returns the calculated amount of keypresses since the last update
    /// @param key The id of the key
    /// @see KeyType
    /// @see update()
    unsigned int getKey(KeyType key);

    /// Clears the buffer of the registered keys
    void clear();

    /// Repeat mode
    bool repeat_mode;

private:

    /// This class stores each keypress
    struct KeyPress {

        /// Constructor
        KeyPress(
	        unsigned int repeat_interval
        ):
            interval(repeat_interval),
            curr_time(0),
            pressed(0)
        {}

        unsigned int interval, curr_time;
        unsigned int pressed;

    };
    /// Map to store our keypresses
    std::map<int, KeyPress*> key_store;
};

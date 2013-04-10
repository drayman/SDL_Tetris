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

    /// Call this "presses the button". If key repeat is disbled,
    /// releaseKey needs to be called before the key can be pressed again.
    /// @param key The id of the key
    /// @see KeyType
    /// @see releaseKey
    void pressKey(KeyType key);

    /// If key repeat is disabled, this needs to be called manually
    /// before calling any pressKey again.
    /// @param key The id of the key
    /// @see KeyType
    /// @see pressKeyKey
    void releaseKey(KeyType key);

    /// Returns the amount of keypresses since the last update
    /// The call sets the counter to zero
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
            pressed(0),
            released(true)
        {}

        unsigned int interval;      ///< Time units before the button can be pressed again
        unsigned int curr_time;     ///< Time left until can be pressed again
        unsigned int pressed;       ///< Amount of keypresses since the last update
        bool released;              ///< Used if key repeat is disabled
    };
    /// Map to store our keypresses
    std::map<int, KeyPress*> key_store;
};

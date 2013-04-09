/** Menu Values
 *
 *  Here we define the Sub-menu categories as MenuType,
 *  and the possible button return values we use as ButtonValue.
 *
 *  Since enum class can't be polymorph, the plan was to forward-declare only them
 *  in the menu.h so the menu actually doesn't need to know about it's values.
 *  We could define these values right where we plan to use them without including
 *  them everywhere we use the menu. C++11 gives us this incredibly useful feature
 *  (or at least useful where enums are generated stuff with tons of values)
 *  but this time my linux gcc isn't mature enough to compile it...Damn it.
 */


/// Here are the values the menus can have
enum class MenuType : int {
    NONE = 0,
    // Sub-menu categories
    MAIN, GAME, SAVE, OVER, OPTS, MISC
};

/// The values the menubuttons can return with
enum class ButtonValue : int {
    NONE = 0,
    // General values
    BACK,
    // Main menu
    NEWGAME, OPTIONS, EXIT,
    // Game
    GAME_LEFT, GAME_RIGHT, GAME_DOWN, GAME_ROTATE,
    // Save
    SAVE_YES, SAVE_NO,
    // Options
    OPTS_TEXTURES, OPTS_MYSTERY, OPTS_FULLSCREEN, OPTS_KEYS, OPTS_FPS
};

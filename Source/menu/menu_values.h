/// Here are the values the menus can have
enum class MenuType : int {
    MAIN = 0,
    GAME, SAVE, OVER, OPTS, MISC
};

/// The values the menubuttons can return with
enum class ButtonValue {
    // General values
    NONE, BACK,
    // Main menu
    NEWGAME, OPTIONS, EXIT,
    // Game
    GAME_LEFT, GAME_RIGHT, GAME_DOWN, GAME_ROTATE,
    // Save
    SAVE_YES, SAVE_NO,
    // Options
    OPTS_TEXTURES, OPTS_MYSTERY, OPTS_FULLSCREEN, OPTS_KEYS, OPTS_FPS
};

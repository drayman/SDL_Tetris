#include "system/view.h"

class DisplayLayout {

public:

    DisplayLayout() :
        fullscreenWidth(1), fullscreenHeight(1),
        windowWidth(1), windowHeight(1),
        landscape(false), fullscreen(false)
    {}

    void setFullscreenSize(GLuint display_width, GLuint display_height) {
        fullscreenWidth = display_width;
        fullscreenHeight = display_height;
    }

    void setWindowSize(GLuint window_width, GLuint window_height) {
        windowWidth = window_width;
        windowHeight = window_height;
    }

    void fullscreenMode(GLuint display_width = 0, GLuint display_height = 0);
    void windowMode(GLuint window_width = 0, GLuint window_height = 0);
    void activeMode(GLuint set_width, GLuint set_height);
    void toggleMode();


    void selectFull();
    void selectMain();
    void selectGame();
    void selectNext();
    void selectControls();
    void selectPanel();

    bool isLandscape() {
        return landscape;
    }

    bool isPortrait() {
        return !landscape;
    }

    bool isFullscreen() {
        return fullscreen;
    }

    bool isWindow() {
        return !fullscreen;
    }

    GLuint width, height;                      ///< Actual width and height

    GLuint fullscreenWidth, fullscreenHeight;  ///< Fullscreen dimensions

    GLuint windowWidth, windowHeight;          ///< Window dimensions

    GLuint mainWidth, mainHeight;              ///< Length of panel(1/3) plus the main game area(2/3)

    GLuint controlWidth, controlHeight;        ///< Dimensions of the menu

private:


    /// reinitializes the projection matrices
    void setSize();

    /// Orientation
    bool landscape;

    /// Fullscren
    bool fullscreen;

    /// Saved projection matrices and viewports
    View view;
};


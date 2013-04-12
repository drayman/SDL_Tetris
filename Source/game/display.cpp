#ifndef DISPLAY_H
#define DISPLAY_H

#include "platform.h"

#include "display.h"


void DisplayLayout::fullscreenMode(GLuint display_width, GLuint display_height)
{
    if ( display_width > 0 && display_height > 0 ) {
        fullscreenWidth = display_width;
        fullscreenHeight = display_height;
    }
    fullscreen = true;
    setSize();
}


void DisplayLayout::windowMode(GLuint window_width, GLuint window_height)
{
    if ( window_width > 0 && window_height > 0 ) {
        windowWidth = window_width;
        windowHeight = window_height;
    }
    fullscreen = false;
    setSize();
}

void DisplayLayout::activeMode(GLuint set_width, GLuint set_height)
{
    if (fullscreen) {
        fullscreenWidth = set_width;
        fullscreenHeight = set_height;
    } else {
        windowWidth = set_width;
        windowHeight = set_height;
    }
    setSize();
}

void DisplayLayout::toggleMode()
{
    fullscreen = !fullscreen;
    setSize();
}


void DisplayLayout::setSize()
{
    if (fullscreen) {
        width=fullscreenWidth;
        height=fullscreenHeight;
    } else {
        width=windowWidth;
        height=windowHeight;
    }

    if (width>height) {
        landscape = true;
        mainWidth=mainHeight=height;
        controlWidth=width-height;
        controlHeight=height;
    } else {
        landscape = false;
        mainHeight=mainWidth=width;
        controlHeight=height-width;
        controlWidth=width;
    }

    view.setPerspective(ViewType::FULL, 90.0f, (float)width/height, 0.1f, 2.0f);
    view.setViewport(ViewType::FULL,
        0, 0, width, height);

    view.setPerspective(ViewType::GAME, 90.0f, 0.667f, 0.1f, 50.0f);
    view.setViewport(ViewType::GAME,
        mainWidth/3, height-mainHeight, mainWidth*2/3, mainHeight);

    view.setPerspective(ViewType::MAIN, 90.0f, 1.0, 0.1f, 50.0f);
    view.setViewport(ViewType::MAIN,
        0, height-mainHeight, mainWidth, mainHeight);

    view.setPerspective(ViewType::NEXT, 90.0f, 1.0, 0.1f, 50.0f);
    view.setViewport(ViewType::NEXT,
        mainWidth/21, height-mainHeight/3+mainHeight/30, mainWidth/4, mainHeight/4);

    view.setOrtho(ViewType::PANEL, 100, 300);
    view.setViewport(ViewType::PANEL, 0, height-mainHeight, mainWidth/3, mainHeight);


    if (landscape)
    {
        view.setOrtho(ViewType::CONTROL, 2, 3);
        view.setViewport(ViewType::CONTROL, mainWidth, 0, controlWidth, controlHeight);
    } else {
        view.setOrtho(ViewType::CONTROL, 3, 2);
        view.setViewport(ViewType::CONTROL, 0, 0, width, height-mainHeight);
    }
}


void DisplayLayout::selectFull()
{
    view.activate(ViewType::FULL);
}

void DisplayLayout::selectMain()
{
    view.activate(ViewType::MAIN);
}

void DisplayLayout::selectGame()
{
    view.activate(ViewType::GAME);
}

void DisplayLayout::selectNext()
{
    view.activate(ViewType::NEXT);
}

void DisplayLayout::selectPanel()
{
    view.activate(ViewType::PANEL);
}

void DisplayLayout::selectControls()
{
    view.activate(ViewType::CONTROL);
}

#endif // DISPLAY_H


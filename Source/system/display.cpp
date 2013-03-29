#ifndef DISPLAY_H
#define DISPLAY_H

#include "platform.h"

#include "display.h"
#include "projection.h"


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
    }
    else {
        landscape = false;
        mainHeight=mainWidth=width;
        controlHeight=height-width;
        controlWidth=width;
    }
    Projection::createPerspective(PPFullMatrix, 90.0f, (float)width/height, 0.1f, 2.0f);
    Projection::createPerspective(PPGameMatrix, 90.0f, 0.667f, 0.1f, 50.0f);
    Projection::createPerspective(PPSquareMatrix, 90.0f, 1.0, 0.1f, 50.0f);

    Projection::createOrtho(POPanelMatrix, 100, 300);


    if (width>height)
        Projection::createOrtho(POControlsMatrix, 2, 3);
    else
        Projection::createOrtho(POControlsMatrix, 3, 2);
}


void DisplayLayout::selectGame()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(PPGameMatrix);
    glViewport(mainWidth/3, height-mainHeight, mainWidth*2/3, mainHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void DisplayLayout::selectMain()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(PPSquareMatrix);
    glViewport(0, height-mainHeight, mainWidth, mainHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void DisplayLayout::selectFull()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(PPFullMatrix);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void DisplayLayout::selectNext()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(PPSquareMatrix);
    glViewport(mainWidth/21, height-mainHeight/3+mainHeight/30, mainWidth/4, mainHeight/4);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void DisplayLayout::selectPanel()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(POPanelMatrix);
    glViewport(0, height-mainHeight, mainWidth/3, mainHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void DisplayLayout::selectControls()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(POControlsMatrix);
    if (width>height)
        glViewport(mainWidth, 0, controlWidth, controlHeight);
    else
        glViewport(0, 0, width, height-mainHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

#endif // DISPLAY_H


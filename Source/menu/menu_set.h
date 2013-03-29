#ifndef MENU_SET_H
#define MENU_SET_H

#include "texture/texture.h"
#include "menu_button.h"

#include <memory>

struct MenuSet
{

    std::shared_ptr<MenuButton> buttons[2][3];

};

#endif // MENU_SET_H

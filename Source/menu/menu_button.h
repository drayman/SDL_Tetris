#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "texture/texture.h"

#include <memory>

struct MenuButton
{

public:

    MenuButton(
        std::shared_ptr<Texture> button_texture,
        unsigned int return_value
    ):
    texture(button_texture),
    value(return_value)
    {}

    unsigned int getValue() {
        return value;
    }

private:

    std::shared_ptr<Texture> texture;

    unsigned int value;

};

#endif // MENU_BUTTON_H

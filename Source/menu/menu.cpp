#include "platform.h"

#include "menu/menu.h"

#include "log/log.h"

Menu::Menu(unsigned int landscape_x_size, unsigned int landscape_y_size)
{
    x_size = landscape_x_size;
    y_size = landscape_y_size;

    width = 0;
    height = 0;

    colors = true;

    active = static_cast<MenuType>(0);
}


Menu::~Menu()
{
    auto it = menu_store.begin();
    while (it !=menu_store.end())
        delete (it++)->second;
}


void Menu::setSize(unsigned int menu_width, unsigned int menu_height)
{
    width = menu_width;
    height = menu_height;

    landscape = ((width>=height)^(x_size<y_size));

    if (landscape)
        if ( width*x_size > height*y_size )
            buttonMesh.scale((float)(height*x_size)/(width*y_size), 1.0f);
        else
            buttonMesh.scale(1.0f, (float)width*y_size/height*x_size);
    else
        if ( width*y_size > height*x_size )
            buttonMesh.scale((float)(height*y_size)/(width*x_size), 1.0f);
        else
            buttonMesh.scale(1.0f, (float)(width*x_size)/(height*y_size));
}


void Menu::addButton(
    MenuType menu_set,
    std::shared_ptr<Texture> texture_ptr,
    ButtonValue return_value,
    unsigned int landscape_x, unsigned int landscape_y,
    unsigned int portrait_x, unsigned int portrait_y,
    bool* enable_draw)
{
    // create the id we gonna use to acces the menu
    int menu_id = static_cast<int>(menu_set);

    // if the submenu does not exist create it
    if (menu_store.count(menu_id) == 0)
        menu_store[menu_id] = new MenuSet(x_size, y_size);

    // create the button
    auto newButton = std::make_shared<MenuButton>(texture_ptr, return_value, enable_draw);

    // insert it
    menu_store[menu_id]->landscape[landscape_x][landscape_y] = newButton;
    menu_store[menu_id]->portrait[portrait_x][portrait_y] = newButton;
}


ButtonValue Menu::getButtonValue(unsigned int x_pos, unsigned int y_pos)
{
    if (width == 0 || height == 0) return ButtonValue::NONE;

    unsigned int menu_x, menu_y;

    if (landscape) {
        menu_x = x_pos*x_size/width;
        menu_y = (height-y_pos)*y_size/height;
        if (menu_store[static_cast<int>(active)]->landscape[menu_x][menu_y]!=NULL)
            return menu_store[static_cast<int>(active)]->landscape[menu_x][menu_y]->value;
    } else {
        menu_x = x_pos*y_size/width;
        menu_y = (height-y_pos)*x_size/height;
        if (menu_store[static_cast<int>(active)]->portrait[menu_x][menu_y]!=NULL)
            return menu_store[static_cast<int>(active)]->portrait[menu_x][menu_y]->value;
    }
    return ButtonValue::NONE;
}


void Menu::setActive(MenuType active_set) {
    //if (menu_store.count(static_cast<int>(active_set)) > 0)
        active = active_set;
}


MenuType Menu::getActive() {
    return active;
}


void Menu::useColors(bool enable_mesh_colors)
{
    colors = enable_mesh_colors;
}


void Menu::draw()
{
    if (width == 0 || height == 0) return;

    buttonMesh.draw_init(false, colors);

    glPushMatrix();

    // create the id to acces the menuset
    int menu_id = static_cast<int>(active);

    // translate to the first button's center
    glTranslatef( 0.5f,  0.5f,  0.0f);

    // iterate through the buttons
    for (unsigned int y=0; y < ((landscape)?y_size:x_size); y++) {
        for (unsigned int x=0; x < ((landscape)?x_size:y_size); x++) {

            // make a ptr to the button
            auto button_ptr = (landscape)?menu_store[menu_id]->landscape[x][y]
                                         :menu_store[menu_id]->portrait[x][y];

            // check if it exists, and if it's connected then it's enabled
            if (button_ptr != NULL &&               // it's a button here
               (button_ptr->enabled == NULL ||      // it's not connected
               (button_ptr->enabled != NULL && *button_ptr->enabled)))  // it's connected and it's true
            {
                button_ptr->texture->bind();
                buttonMesh.draw_only();
            }

            // move next button in the column
            glTranslatef(1.0f, 0.0f, 0.0f);
        }

        // next row, reset column
        glTranslatef(-(float)((landscape)?x_size:y_size), 1.0f, 0.0f);
    }

    glPopMatrix();

    buttonMesh.draw_close();
}


Menu::MenuSet::MenuSet(unsigned int landscape_x_size, unsigned int landscape_y_size)
:
    landscape_x(landscape_x_size), landscape_y(landscape_y_size)
{
    landscape = new std::shared_ptr<MenuButton>*[landscape_x];
    for(unsigned int i = 0; i < landscape_x; i++)
        landscape[i] = new std::shared_ptr<MenuButton>[landscape_y];

    portrait = new std::shared_ptr<MenuButton>*[landscape_y];
    for(unsigned int i = 0; i < landscape_y; i++)
        portrait[i] = new std::shared_ptr<MenuButton>[landscape_x];
}


Menu::MenuSet::~MenuSet()
{
    for(unsigned int i = 0; i < landscape_x; i++)
        delete[] landscape[i];
    delete[] landscape;

    for(unsigned int i = 0; i < landscape_y; i++)
        delete[] portrait[i];
    delete[] portrait;
}

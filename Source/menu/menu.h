#ifndef MENU_H
#define MENU_H

#include "texture/texture.h"
#include "mesh/button_mesh.h"
#include "menu/menu_values.h"

#include <memory>
#include <vector>
#include <map>

class Menu
{
public:

    /// Constructor with the number of icons per column/row in landscape mode
    Menu(unsigned int Landscape_x_size, unsigned int landscape_y_size);

    /// Destructor
    ~Menu();

    /// Set the size of the menu
    void setSize(unsigned int menu_width, unsigned int menu_height);

    /** Creates a new button.
     *    @param menu_set The target submenu id
     *    @param texture_ptr A shared_ptr pointing to the texture to use
     *    @param return_value MenuValue type, the button returns this value if selected
     *    @param portrait_x X position in landscape mode
     *    @param portrait_x Y position in landscape mode
     *    @param portrait_x X position in portrait mode
     *    @param portrait_x Y position in portrait mode
     *    @param enabled Enable/Disable drawing of this button
     */
    void addButton(
        MenuType menu_set,
        std::shared_ptr<Texture> texture_ptr,
        ButtonValue return_value,
        unsigned int landscape_x, unsigned int landscape_y,
        unsigned int portrait_x, unsigned int portrait_y,
        bool enable_draw = true
    );

    /** Enable/Disable drawing the button
     *  @param menu_set MenuType enum, defined in menu_values.h
     *  @param landscape_x Button's X position in landscape mode
     *  @param landscape_y Button's Y position in landscape mode
     *  @param enable_draw Set drawing of this button
     *  @see MenuType
     */
    void enableButton(
        MenuType menu_set,
        unsigned int landscape_x, unsigned int landscape_y,
        bool enable_draw
    );

    /// Returns the value of the button under the given coordinates
    ButtonValue getButtonValue(unsigned int x_pos, unsigned int y_pos);

    /// Set the active menu for drawing
    /// @param active_set MenuType enum, defined in menu_values.h
    /// @see MenuType
    void setActive(MenuType active_set);

    /// Get the active menu for drawing
    /// @see MenuType
    MenuType getActive();

    /// To override the default colors provided by the button mesh
    void useColors(bool enable_mesh_colors);

    /// Draw the menu activated previously by setActive()
    /// @see setActive
    /// @see MenuType
    void draw();

private:

    /// Represents a button texture and its assigned return value
    struct MenuButton {

        /// Constructor
        MenuButton(
	        std::shared_ptr<Texture> button_texture,
	        ButtonValue return_value,
	        bool enable_draw = true
        ):
            texture(button_texture),
            value(return_value),
            enabled(enable_draw)
        {}

        /// Pointer to the button's texture
        std::shared_ptr<Texture> texture;

        /// The return value of the button
        ButtonValue value;

        /// Enable/Disable drawing of this button
        bool enabled;

    };

    /// MenuButtons in a given submenu
    struct MenuSet {

        /// Constructor
        MenuSet(unsigned int landscape_x_size, unsigned int landscape_y_size);

        /// Destructor
        ~MenuSet();

        /// Buttons in landscape mode
        std::shared_ptr<MenuButton> **landscape;

        /// Buttons in portrait mode
        std::shared_ptr<MenuButton> **portrait;

        /// Keep track of array size - since this struct has no draw member its used only by destructor
        /// Previous wrapped vector implementation with 2 level [] overload was more sympathic
        unsigned int landscape_x, landscape_y;

    };

    /// Vector to keep track of main- and submenus
    std::map<int, MenuSet*> menu_store;

    /// ID of the currently active menu
    MenuType active;

    /// Flag of landscape mode
    bool landscape;

    /// Indicates whether to use the mesh embedded colors
    bool colors;

    /// Number of buttons in landscape mode
    unsigned int x_size, y_size;

    /// Menu dimensions
    unsigned int width, height;

    /// Button model
    ButtonMesh buttonMesh;

};

#endif // MENU_H

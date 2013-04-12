#include <map>

#include "system/view_values.h"
//enum class ViewType;

class View {

public:

    /// Creates symmetrical perspective projection, code stolen from:
    /// http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
    void setPerspective(ViewType view_type, GLfloat fovy, GLfloat aspect, GLfloat z_near, GLfloat z_far);

    /// Creates an Ortho View
    /// Modified orthographical projection (not symmetrical, simplified) from:
    /// http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
    void setOrtho(ViewType view_type, GLfloat width, GLfloat height);

    /// Set up the viewport which the projection will be used within
    void setViewport(ViewType view_type, GLuint offset_x, GLuint offset_y, GLuint width, GLuint height);

    /// Activates the previously configured view.
    /// It loads the saved projection matrix, reset the modelview matrix, and sets the viewport
    /// @param view_type The type(id) of the view
    /// @see ViewType
    /// @see setPerspective()
    /// @see setOrtho()
    /// @see setViewport()
    void activate(ViewType view_type);

private:

    struct SavedView {
        /// Display properties
        GLuint x, y, w, h;
        /// Pre-calculated projection matrix
        GLfloat matrix[16];
    };

    std::map<int, SavedView> saved_views;

};

#ifndef MESH_H
#define MESH_H

#include "platform.h"

#include <vector>


/**  Base mesh interface to store and draw geometry with attributes.
 *
 *   At least the meshArray "vertices" needs to be initialized. All arrays are float.
 *   For a 2-dimensional mesh, set "float_per_vertex" to 2. Default is 3.
 *   To declare anti-clockwise order, set "winding" to GL_CW. Default is GL_CCW.
 *   The following arrays can also be defined: normals, colors, texcords.
 *   This is not an abstract class - Can be instantiated to construct meshes from scratch.
 *   For a simple example of creating a derived mesh, please refer to cube_mesh.h file.
 */
struct Mesh {

    typedef std::vector<GLfloat> meshArray;

    /// Constructor to create empty mesh
    Mesh() : floatPerVertex(3), winding(GL_CCW) {}

    /// Destructor to erase the arrays
    virtual ~Mesh() {}

    /// Returns true if the mesh has normals attached
    GLboolean hasNormals() {
        return (normals.size()>0);
    }

    /// Returns true if the mesh has colors attached
    GLboolean hasColors() {
        return (colors.size()>0);
    }

    /// Returns true if the mesh has texture coordinates attached
    GLboolean hasTexices() {
        return (texices.size()>0);
    }

    /// Get the number of vertices
    GLuint getVertexCount() {
        return vertices.size()/floatPerVertex;
    }

    /// Returns true if the mesh is scaled
    GLboolean isScaled() {
        return (scaledVertices.size()>0);
    }

    /// Creates scaled vertex array
    void scale(GLfloat x, GLfloat y, GLfloat z=1.0f);

    /// Disables scaling functionality
    void resetScale();

    /// Draws the mesh
    virtual void draw(
        GLboolean enable_normals = true,
        GLboolean enable_colors = true,
        GLboolean enable_textures = true,
        GLuint vertex_offset = 0,
        GLuint vertices_to_draw = 0
    );

    /// Set up drawing for multiple objects. Use with "draw_only" and "draw_close" methods
    virtual void draw_init(
        GLboolean enable_normals = true,
        GLboolean enable_colors = true,
        GLboolean enable_textures = true,
        GLuint vertex_offset = 0,
        GLuint vertices_to_draw = 0
    );

    /// Draws the object previously initialized with "draw_init". Finish drawing by calling "draw_close"
    virtual void draw_only();

    /// Close drawing of multiple objects. Use with "draw_init" and "draw_only" methods
    virtual void draw_close();

    meshArray vertices;         ///< Vertex positions
    meshArray scaledVertices;   ///< Scaled vertices
    meshArray normals;          ///< Vertex normals
    meshArray colors;           ///< Vertex colors
    meshArray texices;          ///< Vertex texture coordinates

    GLubyte floatPerVertex;     ///< Float/Vertex in vertices
    GLuint winding;             ///< GL_CCW or GL_CW

private:
    GLuint verticesToDraw;      ///< Used by draw()

};

#endif // MESH_H

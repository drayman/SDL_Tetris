#ifndef PROJECTION_H
#define PROJECTION_H

#include "platform.h"

class Projection {

public:

    /// creates symmetrical perspective projection:
    /// http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
    static void createPerspective(
        GLfloat projection_matrix[],
        GLfloat fovy, GLfloat aspect,
        GLfloat z_near, GLfloat z_far );

    // using modified orthographical projection (not symmetrical, simplified):
    // http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
    static void createOrtho(
        GLfloat projection_matrix[],
        GLfloat width, GLfloat height);
};

#endif // PROJECTION_H

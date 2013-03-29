#include "mesh.h"

#include "log/log.h"

void Mesh::scale(GLfloat x, GLfloat y, GLfloat z)
{
    if (isScaled()) scaledVertices.clear();
    auto it = vertices.begin();
    while (it != vertices.end()) {
        scaledVertices.push_back((*it++)*x);
        scaledVertices.push_back((*it++)*y);
        if (floatPerVertex==3)
            scaledVertices.push_back((*it++)*z);
    }
}


void Mesh::resetScale()
{
    if (isScaled()) scaledVertices.clear();
}


void Mesh::draw(
    GLboolean enable_normals,
    GLboolean enable_colors,
    GLboolean enable_textures,
    GLuint vertex_offset,
    GLuint vertices_to_draw )
{
    draw_init(enable_normals, enable_colors, enable_textures, vertex_offset, vertices_to_draw);
    draw_only();
    draw_close();
}

void Mesh::draw_init(
    GLboolean enable_normals,
    GLboolean enable_colors,
    GLboolean enable_textures,
    GLuint vertex_offset,
    GLuint vertices_to_draw )
{
#   ifdef DEBUG_MODE
    if (getVertexCount()==0) {
        LOGW("Drawing empty mesh!");
        return;
    }

    if (getVertexCount()<vertices_to_draw) {
        LOGW("Too few vertices to draw!");
        return;
    }
#   endif

    if (vertices_to_draw==0)
        verticesToDraw = getVertexCount();
    else
        verticesToDraw = vertices_to_draw;

    glFrontFace(winding);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (isScaled())
        glVertexPointer(floatPerVertex, GL_FLOAT, 0, &scaledVertices[vertex_offset*floatPerVertex]);
    else
        glVertexPointer(floatPerVertex, GL_FLOAT, 0, &vertices[vertex_offset*floatPerVertex]);

    if (enable_normals && hasNormals()) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, &normals[vertex_offset*3]);
    }

    if (enable_colors && hasColors()) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, &colors[vertex_offset*4]);
    }

    if (enable_textures && hasTexices()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &texices[vertex_offset*2]);
    }
}


void Mesh::draw_only()
{
    glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);

#   ifdef DEBUG_MODE    // Check for errors
    for (GLint error = glGetError(); error; error = glGetError())
        LOGW("glError (0x%x) at base mesh drawing", error);
#   endif
}


void Mesh::draw_close()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

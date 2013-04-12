#include "platform.h"
#include "view.h"


// [ 0 4 8 12 ]        Right-winding system,
// [ 1 5 9 13 ]        inverse y-texcoords,
// [ 2 6 10 14 ]       column-continous matrices,
// [ 3 7 11 15 ]       oh, I <3 OpenGL
void View::setPerspective(ViewType view_type,
    GLfloat fovy, GLfloat aspect, GLfloat z_near, GLfloat z_far)
{
#   define M_PI_360                            0.008726646
    GLfloat half_height = z_near * tan( fovy * M_PI_360 );
    GLfloat half_width = half_height * aspect;

    SavedView* sv = &saved_views[static_cast<int>(view_type)];
    for(int i=0; i<16; i++)
        sv->matrix[i]=0.0f;
    sv->matrix[ 0] = 2.0f * z_near / ( half_width*2.0f );
    sv->matrix[ 5] = 2.0f * z_near / ( half_height*2.0f );
    sv->matrix[10] = -( z_far + z_near ) / ( z_far - z_near );
    sv->matrix[14] = -( 2.0f * z_far * z_near ) / ( z_far - z_near );
    sv->matrix[11] = -1.0f;
}


void View::setOrtho(ViewType view_type,
    GLfloat width, GLfloat height)
{
    SavedView* sv = &saved_views[static_cast<int>(view_type)];
    for(int i=0; i<16; i++) 
        sv->matrix[i]=0.0f;
    sv->matrix[ 0] =  2.0f/width;
    sv->matrix[ 5] =  2.0f/height;
    sv->matrix[10] = -1.0f;
    sv->matrix[12] = -1.0f;
    sv->matrix[13] = -1.0f;
    sv->matrix[15] =  1.0f;
}

void View::setViewport(ViewType view_type,
    GLuint offset_x, GLuint offset_y, GLuint width, GLuint height)
{
    SavedView* sv = &saved_views[static_cast<int>(view_type)];
    sv->x = offset_x;
    sv->y = offset_y;
    sv->w = width;
    sv->h = height;
}

void View::activate(ViewType view_type)
{
    SavedView* sv = &saved_views[static_cast<int>(view_type)];

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(sv->matrix);
    glViewport(sv->x, sv->y, sv->w, sv->h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


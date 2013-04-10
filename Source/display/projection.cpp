#include "projection.h"

void Projection::createPerspective(
    GLfloat projection_matrix[],
    GLfloat fovy, GLfloat aspect,
    GLfloat z_near, GLfloat z_far )
{
#   define M_PI_360                            0.008726646
    GLfloat half_height = z_near * tan( fovy * M_PI_360 );
    GLfloat half_width = half_height * aspect;

    // [ 0 4 8 12 ]        Right-winding system,
    // [ 1 5 9 13 ]        inverse y-texcoords,
    // [ 2 6 10 14 ]       column-continous matrices,
    // [ 3 7 11 15 ]       oh, I <3 OpenGL

    for(int i=0; i<16; i++) projection_matrix[i]=0;
    projection_matrix[ 0 ] = 2 * z_near / ( half_width*2.0f );
    projection_matrix[ 5 ] = 2 * z_near / ( half_height*2.0f );
    projection_matrix[ 10 ] = -( z_far + z_near ) / ( z_far - z_near );
    projection_matrix[ 14 ] = -( 2 * z_far * z_near ) / ( z_far - z_near );
    projection_matrix[ 11 ] = -1;
}


void Projection::createOrtho(
    GLfloat projection_matrix[],
    GLfloat width, GLfloat height)
{
    for(int i=0; i<16; i++) projection_matrix[i]=0.0f;
    projection_matrix[ 0 ] = 2.0f / width;
    projection_matrix[ 5 ] = 2.0f / height;
    projection_matrix[ 10 ] = -1.0f;
    projection_matrix[ 12 ] = -1.0f;
    projection_matrix[ 13 ] = -1.0f;
    projection_matrix[ 15 ] = 1.0f;
}

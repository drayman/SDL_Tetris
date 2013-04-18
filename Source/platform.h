#ifndef PLATFORM_H
#define PLATFORM_H


#if defined __IPHONEOS__ || defined __ANDROID__
#   define MOBILE_DETECTED
#else
#   define DESKTOP_DETECTED
#endif


// include SDL headers
#include "SDL2/SDL.h"

/*
// include the correct GL headers
#ifdef MOBILE_DETECTED
#    include <GLES/gl.h>

#else
#    include <GL/gl.h>
*/
#include "SDL2/SDL_opengl.h"

// Some windows GL implements GL_BGR, but does not define it...
// cut from GL/glext.h
#ifndef GL_BGR
#define GL_BGR                            0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA                           0x80E1
#endif
// it's missing from mingw probably some compatibility issues
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE                  0x812F
#endif

#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP                0x8191
#endif

//#endif

#endif //PLATFORM_H

#ifndef TEXTURE_H
#define TEXTURE_H

#include "platform.h"

#include <string>

class Texture {

public:

    /// Constructor to create empty texture
    Texture() :
        fileName("[Empty texture]"), imageData(NULL),
        Bpp(0), width(0), height(0),
        type(GL_UNSIGNED_BYTE), format(GL_RGB),
        texID(0) {}

    /// Builds GL texture with the attributes from raw image data
    void build();

    /// Activates the texture. Use after glEnable(GL_TEXTURE_2D).
    void bind();

    /// Release the GL texture, resets the attributes and clears the image raw data
    void reset();

    /// Shows whether it's a drawable, initialized texture
    bool isInitialized() { return initialized; }

    /// Frees the needless raw data of the image
    virtual void clear_image_data() = 0;

    /// Loader function

    /** exception will be thrown, if something goes wrong
      * so the caller will decide how to handle the error
      * If build fails, an empty texture can be assigned
      */
    virtual void load(  const std::string& file_name ) = 0;

    /// Base destructor

    /** Must be implemented for image type-specific cleaning.
      * This base destructor cleans out the texture from the GL memory
      * The derived destructor should call clear_image_data() at least.
      */
    virtual ~Texture() = 0;

    std::string fileName;   ///< The path and name of the source image
	GLubyte* imageData;     ///< Image data used for loading and for further processing
	GLuint Bpp;	            ///< Image color depth in byte per pixel
	GLuint width;	        ///< Image width
	GLuint height;          ///< Image height
    GLenum type;            ///< Image type. (Default: GL_UNSIGNED_BYTE)
	GLenum format;          ///< Image format (GL_RGB, GL_BGR, GL_RGBA, GL_BGRA ... )

protected:

    void swap_BGR_RGB();    ///< Modern GL knows BGRA, or at least can swap R and B in the shader. It's not the case here.
    void flip_y();          ///< Targa likes the bottom-left pixel origin. In RLE decompression must flip explicitly

    bool      initialized;  ///< Turns into true, if texture is succesfully loaded and built
	GLuint texID;           ///< GL texture ID used to select the texture

};

#endif // TEXTURE_H

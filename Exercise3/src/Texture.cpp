#include "Texture.h"

#include <iostream>
#include <cstring>

#define cimg_display 0
#include <CImg.h>

using namespace cimg_library;

namespace cgCourse
{
    void Texture::loadFromFile(const std::string &_filename)
    {
        CImg<unsigned char> img(_filename.c_str());

        int channels = img.spectrum();
        size.x = img.width();
        size.y = img.height();

        img.permute_axes("cxyz");

        /*
         *  TODO Task 2 generate the texture here:
         *        - generate the texture handle and set the variable texhandle
         *        - bind the texture handle
         *        - set the min/mag filter parameter and if it works test if you
         *          see a difference between GL_LINEAR, MIPMAP_LINEAR, NEAREST etc..
         *        - set the texture wrap parameters
         *        - set the texture data with glTexImage2D
         *        - generate mipmaps
         *        - unbind the texture handle (we do this to have a clean state when the
         *          function is ending and no textures are bound after this call.)
         *   Note that depending on the channels the a different kind of texture might be created, for 4 channels GL_RGBA, for 3 GL_RGB and for one GL_RED.
         *   Some operating systems might use GL_BGRA
         */

        glGenTextures(1, &texhandle);
        glBindTexture(GL_TEXTURE_2D, texhandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        switch(channels)
		{
			case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
					break;
			case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data());
					break;
			case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, img.data());
					break;
		}
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,NULL);
        // TODO END
    }

    const GLuint &Texture::getTexHandle() const
    {
        return texhandle;
    }
}
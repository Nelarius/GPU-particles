
#pragma once

#include <GL/glew.h>

namespace ce {

class BufferObject;

class Texture {
    public:
        /// @brief Construct the texture.
        /// @param type The target that you want to bind this object to.
        /// Valid targets are GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY,
        /// GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY,
        /// GL_TEXTURE_BUFFER, GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY.
        /// For more information, see the glBindTexture doc entry
        explicit Texture( GLenum type );
        ~Texture();

        Texture()                               = delete;
        Texture( const Texture& )               = delete;
        Texture( Texture&& )                    = delete;
        Texture& operator=( const Texture& )    = delete;
        Texture& operator=( Texture&& )         = delete;

        void    setStore( GLenum internalFormat, const BufferObject& object );

        void    bind();
        void    unbind();
        GLuint  object() const;

    private:
        GLuint      object_;
        GLint       old_;
        GLenum      type_;
};


}


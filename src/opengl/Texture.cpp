#include <opengl/Texture.h>
#include <opengl/BufferObject.h>
#include <opengl/Enum.h>

using ce::BufferObject;
using ce::Texture;

namespace {

/*
 * This function exists, because when querying what is in the current target,
 * we cant't query with the target's GLenum type -  rather, we have to use a variation
 * of it, with the word BINDING in it :(
 * */
/*GLenum GetBindingTarget( GLenum type ) {
    switch( type ) {
        case GL_TEXTURE_BUFFER: return GL_TEXTURE_BINDING_BUFFER;
    }
}*/

}

Texture::Texture( GLenum type )
    :   object_( 0 ),
        old_( 0 ),
        type_( type ) {
    glGenTextures( 1, &object_ );
}

Texture::~Texture() {
    glDeleteTextures( 1, &object_ );
}

void Texture::setStore( GLenum internalFormat, const BufferObject& object ) {
    this->bind();
    glTexBuffer( type_, internalFormat, object.object() );
    this->unbind();
}

void Texture::bind() {
    glGetIntegerv( ce::GetBindingTarget( type_ ), &old_ );
    glBindTexture( type_, object_ );
}

void Texture::unbind() {
    glBindTexture( type_, old_ );
}

GLuint Texture::object() const {
    return object_;
}

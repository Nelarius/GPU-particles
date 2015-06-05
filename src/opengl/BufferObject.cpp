#include <opengl/BufferObject.h>
#include <opengl/Enum.h>

using ce::BufferObject;

BufferObject::BufferObject( GLenum type )
    :   type_( type ) {
    glGenBuffers( 1, &object_ );
}

BufferObject::~BufferObject() {
    glDeleteBuffers( 1, &object_ );
}

void BufferObject::dataStore( GLsizeiptr size, const GLvoid* data, int usage ) {
    size_ = size;
    this->bind();
    glBufferData( type_, size_, data, usage );
    this->unbind();
}

void* BufferObject::mapBufferRange(GLintptr offset, GLsizeiptr length, int accessFlag ) {
    this->bind();
    return glMapBufferRange( type_, offset, length, accessFlag );
}

void BufferObject::unmapBuffer() {
    glUnmapBuffer( type_ );
    this->unbind();
}

void BufferObject::bind() {
    glGetIntegerv( ce::GetBindingTarget( type_ ), &old_ );
    glBindBuffer( type_, object_ );
}

void BufferObject::unbind() {
    glBindBuffer( type_, old_ );
}

GLuint BufferObject::object() const {
    return object_;
}

GLenum BufferObject::type() const {
    return type_;
}

GLsizei BufferObject::size() const {
    return size_;
}

#include <opengl/VertexBuffer.h>
#include <GL/glew.h>

using ce::VertexBuffer;
using ce::VBOAttribute;

VertexBuffer::VertexBuffer()
    :   BufferObject( GL_ARRAY_BUFFER ) {
    glGenVertexArrays( 1, &arrayObject_ );
}

VertexBuffer::~VertexBuffer() {
    glDeleteVertexArrays( 1, &arrayObject_ );
}

void VertexBuffer::addStandardAttribute( int flag ) {
    GLint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, &program );
    
    for ( int i = 0; i < VBOAttribute::Count; i++ ) {
        if ( flag & VBOAttribute::Position ) {
            GLint attrib = glGetAttribLocation( program, "vert" );
            this->addAttribute(
                attrib, 3, GL_FLOAT, GL_FALSE, 0, 0u
            );
        } else if ( flag & VBOAttribute::Normal ) {
            GLint attrib = glGetAttribLocation( program, "norm" );
            this->addAttribute(
                attrib, 3, GL_FLOAT, GL_FALSE, 0, 0u    // THERE MUST BE AN OFFSET
            );
        } else if ( flag & VBOAttribute::Color ) {
            GLint attrib = glGetAttribLocation( program, "color" );
            this->addAttribute(
                attrib, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0u
            );
        }
    }
}

void VertexBuffer::addAttribute(
                                    GLuint index,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLsizei stride,
                                    std::size_t offset
                                ) {
    attributes_[ attribPos_++ ] = VertexAttribute(
                                    index,
                                    size,
                                    type,
                                    normalized,
                                    stride,
                                    offset
                                );
}

void VertexBuffer::bindVao() {
    glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &old_ );
    glBindVertexArray( arrayObject_ );
}

void VertexBuffer::unbindVao() {
    glBindVertexArray( old_ );
}

/*
 * The GL_ARRAY_BUFFER_BINDING is NOT part of the VAO state.
 * The attribute index will get its data from whatever object is currently bound
 * to the target. This association is made when glVertexAttribPointer is called. Only
 * a call to this function can change the object association.
 * */
void VertexBuffer::enableAttributes() {
    this->bind();
    this->bindVao();
    for ( std::size_t i = 0u; i < attribPos_; i++ ) {
        this->enableAttribute_( i );
    }
    this->unbindVao();
    this->unbind();
}

void VertexBuffer::enableAttribute_( std::size_t i ) {
    const VertexAttribute& attrib = attributes_[i];
    glVertexAttribPointer(
        attrib.index,
        attrib.size,
        attrib.type,
        attrib.normalized,
        attrib.stride,
        ( const GLvoid* ) attrib.offset
    );
    glEnableVertexAttribArray( attrib.index );
}

void VertexBuffer::disableAttributes() {
    for ( std::size_t i = 0u; i < attribPos_; i++ ) {
        this->disableAttribute_( i );
    }
}

void VertexBuffer::disableAttribute_( std::size_t i ) {
    const VertexAttribute& attrib = attributes_[i];
    glDisableVertexAttribArray( attrib.index );
}



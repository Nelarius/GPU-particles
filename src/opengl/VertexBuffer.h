
#pragma once

#include <opengl/BufferObject.h>
#include <cstdlib>    //for std::size_t

#define MAX_VBO_ATTRIBUTES 16

namespace ce {

enum VBOAttribute {
    Position      = (1 << 0),
    Normal      = (1 << 1),
    Color       = (1 << 2),
    Count       = 3
};

/**
 * @brief A buffer object suitable to use as a source of vertex data.
 * An example of the use of this class might be the following:
 * @code
 * VertexBuffer buffer{};
 * buffer.dataStore( 10, data, GL_STATIC_DRAW );
 * buffer.addAttribute( shader->attribute("vertex"), 4, GL_FLOAT, GL_FALSE, 0, 0 );
 * buffer.enableAttributes();
 * //later, in drawing code
 * shader->use();
 * buffer.bind();
 * glDrawArrays(...);
 * buffer.unbind();
 * shader->stopUsing();
 * @endcode
 * */
class VertexBuffer : public BufferObject {
    public:
        VertexBuffer();
        // TODO: create initializer-list constructor
        ~VertexBuffer();

        void bindVao();
        void unbindVao();

        void addAttribute(
            GLuint index, GLint size, GLenum type,
            GLboolean normalized, GLsizei stride, std::size_t offset = 0u
        );

        /**
         * @brief Add A standard attribute such as vertex, normal, or color.
         * TODO: this method is still under construction.
         * The number of elements contained in the buffer object must be specified.
         * @param flag The logical combination of VBOAttribute enums.
         */
        void addStandardAttribute( int flag );

        void enableAttributes();
        void disableAttributes();

    private:
        void enableAttribute_( std::size_t index );
        void disableAttribute_( std::size_t index );

        struct VertexAttribute {
            GLuint          index;
            GLint           size;
            GLenum          type;
            GLboolean       normalized;
            GLsizei         stride;
            std::size_t     offset;

            VertexAttribute() = default;
            VertexAttribute(
                GLuint _index,
                int _size,
                GLenum _type,
                bool norm,
                GLsizei _stride,
                std::size_t _offset
            )
            :   index( _index ),
                size( _size ),
                type( _type ),
                normalized( norm ),
                stride( _stride ),
                offset( _offset )
                {}
        };

        VertexAttribute attributes_[MAX_VBO_ATTRIBUTES];
        GLuint          arrayObject_{ 0u };
        GLint           old_{ 0 };
        std::size_t     attribPos_{ 0u };
};

}


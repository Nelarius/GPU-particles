
#pragma once

#include <GL/glew.h>

namespace ce {

/// @brief A wrapper for an OpenGL buffer object, living on GPU memory.
class BufferObject {
    public:
        
        /**
         * @brief Construct the BufferObject with the type of target it should be bound to.
         * @param type The type of target this buffer object will be bound to.
         * The following are valid buffer targets: GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 
         * GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_PIXEL_PACK_BUFFER, 
         * GL_QUERY_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER, 
         * GL_DRAW_INDIRECT_BUFFER, GL_ATOMIC_COUNTER_BUFFER,  GL_DISPATCH_INDIRECT_BUFFER, 
         * GL_SHADER_STORAGE_BUFFER.
         *
         * See https://www.opengl.org/wiki/Buffer_Object for more information.
         */
        explicit BufferObject( GLenum type );
        virtual ~BufferObject();

        BufferObject()                              	= delete;
        BufferObject( const BufferObject& )           	= delete;
        BufferObject& operator=( const BufferObject&) 	= delete;
        BufferObject( BufferObject&& )					= delete;
        BufferObject& operator=( BufferObject&& )   	= delete;

        /// @brief Define the data store and optionally copy data to the store.
        /// @param size The size in bytes of the new data store
        /// @param data A pointer to the data which will be copied into the store. Nothing is copied if it is NULL.
        /// @param usage Specifies the expected data store usage pattern.
        /// This method should be called with a null pointer before the buffer object is mapped to our memory space.
        /// The usage flag can be one of the following: GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
        /// GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_WRITE, GL_DYNAMIC_READ, GL_DYNAMIC_COPY.
        /// For more information, see glBufferData doc entry.
        void    dataStore( GLsizeiptr size, const GLvoid* data, int usage );

        /// @brief Map the data store memory range to local memory space.
        /// @param offset The starting offset within the buffer to be mapped.
        /// @param length The length of the range to be mapped.
        /// @param accessFlag Specify the access pattern of the mapped range using a combination of flags.
        /// accessFlag can be a combination of the following: GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_PERISTENT_BIT,
        /// GL_MAP_COHERENT_BIT, GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT, GL_MAP_FLUSH_EXPLICIT_BIT,
        /// GL_MAP_UNSYNCHRONIZED_BIT. See glMapBufferRange doc entry for more info.
        void*   mapBufferRange( GLintptr offset, GLsizeiptr length, int accessFlag );
        void    unmapBuffer();

        void    bind();
        void    unbind();

        GLuint  object() const;
        GLenum  type() const;
        GLsizei size() const;

    protected:
        GLenum      type_{ GL_ARRAY_BUFFER };  // the target that this object will be bound to
        GLuint      object_{ 0u };    // the OpenGL name of this object
        GLsizei     size_{ 0u };    // the size of the data store in bytes
        GLint       old_{ 0u };
};


}


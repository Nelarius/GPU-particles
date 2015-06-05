#ifndef ENUM_OPENGL_H
#define ENUM_OPENGL_H

namespace ce {

inline GLenum GetBindingTarget( GLenum type ) {
    switch( type ) {
        case GL_ARRAY_BUFFER:               return GL_ARRAY_BUFFER_BINDING;
        case GL_ELEMENT_ARRAY_BUFFER:       return GL_ELEMENT_ARRAY_BUFFER_BINDING;
        // these don't exist on linux for some reason
        //case GL_COPY_READ_BUFFER:           return GL_COPY_READ_BUFFER_BINDING;
        //case GL_COPY_WRITE_BUFFER:          return GL_COPY_WRITE_BUFFER_BINDING;
        case GL_PIXEL_UNPACK_BUFFER:        return GL_PIXEL_UNPACK_BUFFER_BINDING;
        case GL_PIXEL_PACK_BUFFER:          return GL_PIXEL_PACK_BUFFER_BINDING;
        case GL_QUERY_BUFFER:               return GL_QUERY_BUFFER_BINDING;
        case GL_TRANSFORM_FEEDBACK_BUFFER:  return GL_TRANSFORM_FEEDBACK_BINDING;
        case GL_UNIFORM_BUFFER:             return GL_UNIFORM_BUFFER_BINDING;
        case GL_DRAW_INDIRECT_BUFFER:       return GL_DRAW_INDIRECT_BUFFER_BINDING;
        case GL_ATOMIC_COUNTER_BUFFER:      return GL_ATOMIC_COUNTER_BUFFER_BINDING;
        case GL_DISPATCH_INDIRECT_BUFFER:   return GL_DISPATCH_INDIRECT_BUFFER_BINDING;
        case GL_SHADER_STORAGE_BUFFER:      return GL_SHADER_STORAGE_BUFFER_BINDING;
        case GL_TEXTURE_BUFFER:             return GL_TEXTURE_BINDING_BUFFER;
        case GL_TEXTURE_1D:                 return GL_TEXTURE_BINDING_1D;
        case GL_TEXTURE_2D:                 return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_3D:                 return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_1D_ARRAY:           return GL_TEXTURE_BINDING_1D_ARRAY;
        case GL_TEXTURE_2D_ARRAY:           return GL_TEXTURE_BINDING_2D_ARRAY;
        case GL_TEXTURE_RECTANGLE:          return GL_TEXTURE_BINDING_RECTANGLE;
        case GL_TEXTURE_CUBE_MAP:           return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_CUBE_MAP_ARRAY:     return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case GL_TEXTURE_2D_MULTISAMPLE:     return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:   return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
    }

}

}

#endif //ENUM_OPENGL_H

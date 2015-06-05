#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <opengl/Shader.h>
#include <string>

//an interface with the file system
//load resources, etc.
namespace ce {

/// \brief Load a shader from a file.
/// \return Pointer to the shader.
Shader::ptr ShaderFromFile( const std::string& file, GLenum shaderType );

}

#endif // INTERFACE_H_INCLUDED

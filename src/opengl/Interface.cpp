#include <opengl/Interface.h>
#include <utils/File.h>

using ce::Shader;

Shader::ptr ce::ShaderFromFile( const std::string& file, GLenum shaderType ) {
    return Shader::ptr( new Shader( ce::FileToString(file), shaderType ) );
}

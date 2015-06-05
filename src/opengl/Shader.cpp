#include <opengl/Shader.h>
#include <utils/Assert.h>

using ce::Shader;

Shader::Shader( const std::string& shaderCode, GLenum shaderType ) {
    //create the shader object, returns 0 on error
    object_ = glCreateShader( shaderType );
    ASSERT( object_ != 0, "error: glCreateShader() failed." );

    const char* code = shaderCode.c_str();
    glShaderSource( object_, 1, (const GLchar**)& code, NULL );

    glCompileShader( object_ );

    //check for compilation error
    GLint status;
    glGetShaderiv( object_, GL_COMPILE_STATUS, &status );
    if (status == GL_FALSE ) {
        std::string msg("Compile failure\n");
        GLint infoLogLength;
        glGetShaderiv( object_, GL_INFO_LOG_LENGTH, &infoLogLength );
        char* infoLogStr = new char[ infoLogLength + 1 ];
        glGetShaderInfoLog( object_, infoLogLength, NULL, infoLogStr );
        msg += infoLogStr;
        delete [] infoLogStr;

        glDeleteShader( object_ );
        object_ = 0;
        ASSERT( false, msg.c_str() );
    }
}

Shader::~Shader() {
    glDeleteShader( object_ );
    object_ = 0;
}

GLuint Shader::object() const {
    return object_;
}

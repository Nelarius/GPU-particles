#include <opengl/Program.h>
#include <utils/Assert.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>  //for std::exit

using ce::Program;
using ce::Shader;

Program::Program( const std::vector<Shader::ptr>& shaders ) {
    if ( shaders.size() == 0u ) {
        std::cerr << "No shaders were provided to create the program.\n";
        std::exit( EXIT_SUCCESS );
    }

    object_ = glCreateProgram();
    ASSERT( object_ != 0, "error: glCreateProgram() failed." );

    for ( auto& shader: shaders ) {
        glAttachShader( object_, shader->object() );
    }

    glLinkProgram( object_ );

    //check the linking status
    GLint status;
    glGetProgramiv( object_, GL_LINK_STATUS, &status );
    if ( status == GL_FALSE ) {
        std::string msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv( object_, GL_INFO_LOG_LENGTH, &infoLogLength );
        char* infoLogStr = new char[ infoLogLength + 1 ];
        glGetProgramInfoLog( object_, infoLogLength, NULL, infoLogStr );
        msg += infoLogStr;
        delete [] infoLogStr;

        glDeleteProgram( object_ );
        object_ = 0;
        ASSERT( false, msg.c_str() );
    }
}

Program::~Program() {
    glDeleteProgram( object_ );
    object_ = 0;
}

GLuint Program::object() const {
    return object_;
}

GLint Program::attribute( const GLchar* attribName ) const {
    ASSERT( attribName, "error: attribName was NULL" );

    GLint attrib = glGetAttribLocation( object_, attribName );
    ASSERT( attrib != -1, "error: program attribute not found" );

    return attrib;
}

GLint Program::uniform( const GLchar* uniformName ) const {
    ASSERT( uniformName, "error: uniformName was NULL" );

    GLint uniform = glGetUniformLocation( object_, uniformName );
    #ifdef DEBUG
    std::string msg( "error: program uniform " );
    msg += uniformName;
    msg += " not found";
    ASSERT( uniform != -1, msg.c_str() );
    #endif // DEBUG
    return uniform;
}

GLint Program::subroutineUniform( const GLchar* uniformName, GLenum shaderType ) const {
    ASSERT( uniformName, "error: subroutine uniform name was null" );
    GLint uniform = glGetSubroutineUniformLocation( object_, shaderType, uniformName );
    #ifdef DEBUG
    std::string msg( "error: subroutine uniform " );
    msg += uniformName;
    msg += " not found";
    ASSERT( uniform != -1, msg.c_str() );
    #endif  // DEBUG
    return uniform;
}

GLuint Program::subroutineIndex( const GLchar* functionName, GLenum shaderType ) const {
    ASSERT( functionName, "error: subroutine function name was null" );
    GLuint index = glGetSubroutineIndex( object_, shaderType, functionName );
    #ifdef DEBUG
    std::string msg("error: active subroutine " );
    msg += functionName;
    msg += " not found";
    ASSERT( index != GL_INVALID_INDEX, msg.c_str() );
    #endif // DEBUG
    return index;
}

void Program::use() const {
    glUseProgram( object_ );
}

bool Program::isInUse() const {
    GLint currentProgram = 0;
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );
    return ( currentProgram == (GLint) object_ );
}

void Program::stopUsing() const {
    ASSERT( isInUse(), "error: cannot stop using because program is not in use" );
    glUseProgram( 0 );
}

void Program::setUniform( const GLchar* name, GLint v0 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform1i( uniform(name), v0 );
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform2i( uniform(name), v0, v1);
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1, GLint v2 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform3i( uniform(name), v0, v1, v2 );
}

void Program::setUniform( const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform4i( uniform(name), v0, v1, v2, v3 );
}

void Program::setUniform( const GLchar* name, GLfloat v0 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform1f( uniform(name), v0);
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform2f( uniform(name), v0, v1 );
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2 ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform3f( uniform(name), v0, v1, v2 );
}

void Program::setUniform( const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform4f( uniform(name), v0, v1, v2, v3 );
}

void Program::setUniform( const GLchar* name, const glm::vec2& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform2fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const glm::vec3& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform3fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const glm::vec4& v ) const
{
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniform4fv( uniform(name), 1, glm::value_ptr( v ) );
}

void Program::setUniform( const GLchar* name, const glm::mat3& m, GLboolean transpose ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniformMatrix3fv( uniform(name), 1, transpose, glm::value_ptr( m ) );
}

void Program::setUniform( const GLchar* name, const glm::mat4& m, GLboolean transpose ) const {
    ASSERT( isInUse(), "Program::setUniform> program not in use" );
    glUniformMatrix4fv( uniform(name), 1, transpose, glm::value_ptr( m ) );
}


#ifndef PROGRAM_H
#define PROGRAM_H

#include <opengl/Shader.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace ce {

/// \brief Contains a compiled and linked OpenGL shader program.
class Program {
    public:
        typedef std::unique_ptr<Program>  ptr;

        Program() = delete;
        /// \brief Construct the program from a vector of shader objects
        Program( const std::vector<Shader::ptr>& shaders );
        ~Program();

        //make uncopyable for now
        Program( const Program& )               = delete;
        Program( Program&& )                    = delete;
        Program& operator=( const Program& )    = delete;
        Program& operator=( Program&& )         = delete;

        /// \brief Get the OpenGL handle.
        GLuint object() const;

        GLint attribute( const GLchar* attribName ) const;

        GLint uniform( const GLchar* uniformName ) const;

        GLint subroutineUniform( const GLchar* uniformName, GLenum shaderType ) const;

        GLuint subroutineIndex( const GLchar* functionName, GLenum shaderType ) const;

        /// \brief Use this shader.
        void use() const;
        bool isInUse() const;
        void stopUsing() const;
        
        /// A wrapper for glUniform*i
        void setUniform( const GLchar*, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint, GLint ) const;
        void setUniform( const GLchar*, GLint, GLint, GLint, GLint ) const;
        /// A wrapper for glUniform*f
        void setUniform( const GLchar*, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat, GLfloat ) const;
        void setUniform( const GLchar*, GLfloat, GLfloat, GLfloat, GLfloat ) const;
        /// A wrapper for glUniform*fv
        void setUniform( const GLchar*, const glm::vec2& ) const;
        void setUniform( const GLchar*, const glm::vec3& ) const;
        void setUniform( const GLchar*, const glm::vec4& ) const;

        /// A wrapper for glUniformMatrix*fv
        void setUniform( const GLchar*, const glm::mat3&, GLboolean transpose = GL_FALSE ) const;
        void setUniform( const GLchar*, const glm::mat4&, GLboolean transpose = GL_FALSE ) const;

    private:
        GLuint  object_{ 0u };
};


}   //namespace ce

#endif // PROGRAM_H

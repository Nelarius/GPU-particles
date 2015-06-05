#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <memory>

namespace ce {

/// \brief Contains an OpenGL shader object.
class Shader {
    public:
        typedef std::unique_ptr<Shader> ptr;

        Shader() = delete;

        /// \brief Construct a shader from a string source.
        Shader( const std::string& shaderCode, GLenum shaderType );
        ~Shader();

        //make uncopyable for now
        Shader( const Shader& )             = delete;
        Shader( Shader&& )                  = delete;
        Shader& operator=( const Shader& )  = delete;
        Shader& operator=( Shader&& )       = delete;

        /// \brief Get the OpenGL handle.
        GLuint object() const;

    private:
        GLuint object_{ 0u };
};

}

#endif // SHADER_H

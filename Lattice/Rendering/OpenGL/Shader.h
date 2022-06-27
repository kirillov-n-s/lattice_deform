#ifndef LATTICE_RENDERING_OPENGL_SHADER_H
#define LATTICE_RENDERING_OPENGL_SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Lattice::Rendering::OpenGL {
    class Shader
    {
    public:
        Shader(
            const std::string &vertPath,
            const std::string &fragPath);
        ~Shader();

        void use() const;

        template <typename U>
        void setUniform(
            const std::string& name,
            const U &value) const;

    private:
        unsigned int m_id;

        int uniformLocation(const std::string& name) const;

        static void compileShader(
            const unsigned int id,
            const char **code);
    };
}

namespace Lattice::Rendering::OpenGL {
    template <typename>
    constexpr bool is_always_false = false;

    template<typename U>
    void Shader::setUniform(
        const std::string &name,
        const U &value) const
    {
        const int location = uniformLocation(name);
        if constexpr(std::is_same_v<U, float>)
            glUniform1f(location, value);
        else if constexpr(std::is_same_v<U, glm::vec3>)
            glUniform3f(location, value.x, value.y, value.z);
        else if constexpr(std::is_same_v<U, glm::vec4>)
            glUniform4f(location, value.x, value.y, value.z, value.w);
        else if constexpr(std::is_same_v<U, glm::mat4>)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        else if constexpr(std::is_same_v<U, int>)
            glUniform1i(location, value);
        else
            static_assert(is_always_false<U>, "Uniform of that type is not supported.");
    }
}

#endif

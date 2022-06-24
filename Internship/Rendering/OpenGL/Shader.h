#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Internship::Rendering::OpenGL
{
    template <typename>
    constexpr bool is_always_false = false;

    class Shader
    {
    private:
        unsigned int _id;

        int get_uniform_location(const std::string& name) const;

    public:
        Shader(const std::string& vert_path, const std::string& frag_path);
        ~Shader();

        void use() const;

        template <typename U>
        void set_uniform(const std::string& name, U value) const;
    };
}

namespace Internship::Rendering::OpenGL
{
    template<typename U>
    void Shader::set_uniform(const std::string &name, U value) const
    {
        auto location = get_uniform_location(name);
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

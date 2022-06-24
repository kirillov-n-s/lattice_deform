#include "Shader.h"

namespace Internship::Rendering::OpenGL
{
    int Shader::get_uniform_location(const std::string &name) const
    {
        return glGetUniformLocation(_id, name.c_str());
    }

    Shader::Shader(const std::string &vert_path, const std::string &frag_path)
    {
        std::ifstream vert_file, frag_file;

        vert_file.open(vert_path);
        if (!vert_file.is_open())
            throw std::runtime_error(std::string("Cannot open file ") + vert_path);

        frag_file.open(frag_path);
        if (!frag_file.is_open())
            throw std::runtime_error(std::string("Cannot open file ") + frag_path);

        std::ostringstream vert_stream, frag_stream;
        vert_stream << vert_file.rdbuf();
        frag_stream << frag_file.rdbuf();

        vert_file.close();
        frag_file.close();

        std::string vert_string = vert_stream.str();
        std::string frag_string = frag_stream.str();

        const char* vert_code = vert_string.c_str();
        const char* frag_code = frag_string.c_str();

        unsigned int vert, frag;
        int success, length;
        std::vector<char> log;

        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vert_code, nullptr);
        glCompileShader(vert);
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &length);
            log.resize(length);
            glGetShaderInfoLog(vert, length, nullptr, log.data());
            glDeleteShader(vert);
            throw std::runtime_error(std::string("Vertex Shader compile error. Log:\n") + log.data() + '\n');
        }

        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &frag_code, nullptr);
        glCompileShader(frag);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &length);
            log.resize(length);
            glGetShaderInfoLog(frag, length, nullptr, log.data());
            glDeleteShader(frag);
            throw std::runtime_error(std::string("Fragment Shader compile error. Log:\n") + log.data() + '\n');
        }

        _id = glCreateProgram();
        glAttachShader(_id, vert);
        glAttachShader(_id, frag);
        glLinkProgram(_id);
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
            log.resize(length);
            glGetProgramInfoLog(_id, length, nullptr, log.data());
            glDeleteProgram(_id);
            glDeleteShader(vert);
            glDeleteShader(frag);
            throw std::runtime_error(std::string("Program link error. Log:\n") + log.data());
        }

        glDetachShader(_id, vert);
        glDetachShader(_id, frag);
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    Shader::~Shader()
    {
        glDeleteProgram(_id);
    }

    void Shader::use() const
    {
        glUseProgram(_id);
    }
}

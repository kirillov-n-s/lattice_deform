#include "Shader.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace Lattice::Rendering::OpenGL
{
    int Shader::uniformLocation(const std::string &name) const
    {
        return glGetUniformLocation(m_id, name.c_str());
    }

    Shader::Shader(
        const std::string &vertPath,
        const std::string &fragPath)
    {
        std::ifstream vertFile,
                      fragFile;

        vertFile.open(vertPath);
        if (!vertFile.is_open())
            throw std::runtime_error(std::string("Cannot open file ") + vertPath);

        fragFile.open(fragPath);
        if (!fragFile.is_open())
            throw std::runtime_error(std::string("Cannot open file ") + fragPath);

        std::ostringstream vertStream,
                           fragStream;
        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        const std::string vertString = vertStream.str(),
                          fragString = fragStream.str();

        const char* vertCode = vertString.c_str(),
                  * fragCode = fragString.c_str();

        unsigned int vertId,
                     fragId;

        int isSuccess,
            logLength;
        std::vector<char> log;

        auto compileShader =
        [&isSuccess, &logLength, &log](
            const unsigned int id,
            const char **code,
            const char *type)
        {
            glShaderSource(
                id,
                1,
                code,
                nullptr);
            glCompileShader(id);
            glGetShaderiv(
                id,
                GL_COMPILE_STATUS,
                &isSuccess);
            if(!isSuccess) {
                glGetShaderiv(
                    id,
                    GL_INFO_LOG_LENGTH,
                    &logLength);
                log.resize(logLength);
                glGetShaderInfoLog(
                    id,
                    logLength,
                    nullptr,
                    log.data());
                glDeleteShader(id);
                throw std::runtime_error(type + std::string(" shader compilation error. Log:\n") + log.data() + '\n');
            }
        };

        vertId = glCreateShader(GL_VERTEX_SHADER);
        compileShader(
            vertId,
            &vertCode,
            "Vertex");
        fragId = glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(
            fragId,
            &fragCode,
            "Fragment");

        m_id = glCreateProgram();
        glAttachShader(m_id, vertId);
        glAttachShader(m_id, fragId);
        glLinkProgram(m_id);
        glGetProgramiv(m_id, GL_LINK_STATUS, &isSuccess);
        if(!isSuccess) {
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
            log.resize(logLength);
            glGetProgramInfoLog(m_id, logLength, nullptr, log.data());
            glDeleteProgram(m_id);
            glDeleteShader(vertId);
            glDeleteShader(fragId);
            throw std::runtime_error(std::string("Program link error. Log:\n") + log.data());
        }

        glDetachShader(m_id, vertId);
        glDetachShader(m_id, fragId);
        glDeleteShader(vertId);
        glDeleteShader(fragId);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_id);
    }

    void Shader::use() const
    {
        glUseProgram(m_id);
    }
}

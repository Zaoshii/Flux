#pragma once
#include <map>
#include <glad/glad.h>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>

namespace Engine::Rendering
{
    std::string GetShaderContent(std::string path)
{
    std::ifstream input(path, std::ios::in);

    if (!input.is_open() || !input.good())
        throw std::runtime_error("Could not open the specified path");

    std::stringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}


    class Shader
{
private:
    GLuint m_Handle;
    mutable std::unordered_map<std::string, GLint> uniformLocationCache;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath)
    {

        int success;
        char infoLog[512];

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        std::string vertexSource = GetShaderContent(vertexPath);
        std::string fragmentSource = GetShaderContent(fragmentPath);

        const char *vertexCode = vertexSource.c_str();
        const char *fragmentCode = fragmentSource.c_str();

        glShaderSource(vertexShader, 1, &vertexCode, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog));
        }

        glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog));
        }

        m_Handle = glCreateProgram();

        glAttachShader(m_Handle, vertexShader);
        glAttachShader(m_Handle, fragmentShader);
        glLinkProgram(m_Handle);
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_Handle, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::PROGRAM::LINKING::FAILED\n{}", infoLog));
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() { glDeleteProgram(m_Handle); }

    void Use() { glUseProgram(m_Handle); }

    GLuint GetID() { return m_Handle; }

    GLint GetUniformLocation(const std::string &name) const
    {
        if (auto iterator = uniformLocationCache.find(name); iterator != uniformLocationCache.end())
        {
            return iterator->second;
        }
        GLint location = glGetUniformLocation(m_Handle, name.c_str()); // lazy caching
        uniformLocationCache[name] = location;
        return location;
    }

    void SetFloat(const std::string &name, const float data) const
    {
        glUniform1f(GetUniformLocation(name), data);
    }

    void SetVec2(const std::string &name, const glm::vec2 data) const
    {
        glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(data));
    }
};
}
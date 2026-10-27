#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>

namespace Engine::Rendering
{
inline std::string GetShaderContent(std::string path)
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
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

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

    void SetVec3(const std::string &name, const glm::vec3 data) const
    {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(data));
    }

    void SetVec4(const std::string &name, const glm::vec4 data) const
    {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(data));
    }
};
} // namespace Engine::Rendering
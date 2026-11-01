#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>
#include "Core/file/file.hpp"

namespace engine::rendering
{

class Shader
{

public:
    Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);

    ~Shader() { glDeleteProgram(handle_); }

    void use() { glUseProgram(handle_); }

    GLuint getID() { return handle_; }

    GLint getUniformLocation(const std::string &name) const
    {
        if (auto iterator = uniformLocationCache_.find(name);
            iterator != uniformLocationCache_.end())
        {
            return iterator->second;
        }
        GLint location = glGetUniformLocation(handle_, name.c_str()); // lazy caching
        uniformLocationCache_[name] = location;
        return location;
    }

    void setInt(const std::string &name, const int &data) const
    {
        glUniform1i(getUniformLocation(name), data);
    }

    void setFloat(const std::string &name, const float &data) const
    {
        glUniform1f(getUniformLocation(name), data);
    }

    void setVec2(const std::string &name, const glm::vec2 &data) const
    {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(data));
    }

    void setVec3(const std::string &name, const glm::vec3 &data) const
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(data));
    }

    void setVec4(const std::string &name, const glm::vec4 &data) const
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(data));
    }
    void setMat4(const std::string &name, const glm::mat4 &data) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(data));
    }

private:
    GLuint handle_;
    mutable std::unordered_map<std::string, GLint> uniformLocationCache_;
};
} // namespace engine::rendering
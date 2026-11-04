#include "Rendering/Shader.hpp"
namespace engine::rendering
{
Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
{
    int success;
    char infoLog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexSource = core::file::readTextFile(vertexPath);
    std::string fragmentSource = core::file::readTextFile(fragmentPath);

    const char *vertexCode = vertexSource.c_str();
    const char *fragmentCode = fragmentSource.c_str();

    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        glDeleteShader(vertexShader);
        throw std::runtime_error(
            std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog));
    }

    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error(
            std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog));
    }

    handle_ = glCreateProgram();

    glAttachShader(handle_, vertexShader);
    glAttachShader(handle_, fragmentShader);
    glLinkProgram(handle_);
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(handle_, 512, nullptr, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(handle_);
        throw std::runtime_error(
            std::format("ERROR::SHADER::PROGRAM::LINKING::FAILED\n{}", infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

} // namespace engine::rendering
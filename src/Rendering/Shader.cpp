#include "Rendering/Shader.hpp"
namespace engine::rendering
{
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
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

} // namespace engine::rendering
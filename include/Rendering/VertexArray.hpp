#pragma once
#include <glad/glad.h>

namespace engine::rendering
{
class VertexArray
{
private:
    GLuint m_Handle;

public:
    VertexArray() { glGenVertexArrays(1, &m_Handle); }

    ~VertexArray() { glDeleteVertexArrays(1, &m_Handle); }

    void AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        Bind();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
    {
        m_Handle = other.m_Handle;
        other.m_Handle = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept
    {
        if (this != &other)
        {
            if (this->m_Handle != 0)
            {
                glDeleteVertexArrays(1, &this->m_Handle);
            }
            m_Handle = other.m_Handle;
            other.m_Handle = 0;
        }
        return *this;
    }

    void Bind() const { glBindVertexArray(m_Handle); }

    void Unbind() const { glBindVertexArray(0); }
};
} // namespace engine::rendering
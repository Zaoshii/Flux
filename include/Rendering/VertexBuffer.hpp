#pragma once
#include <glad/glad.h>
namespace engine::rendering
{
class VertexBuffer
{
private:
    GLuint m_Handle;

public:
    VertexBuffer() { glGenBuffers(1, &m_Handle); }

    ~VertexBuffer() { glDeleteBuffers(1, &m_Handle); }

    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete; // empêche de copier

    VertexBuffer(VertexBuffer &&other) noexcept
    {
        m_Handle = other.m_Handle;
        other.m_Handle = 0;
    }
    VertexBuffer &operator=(VertexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            if (m_Handle != 0)
            {
                glDeleteBuffers(1, &m_Handle);
            }
            m_Handle = other.m_Handle;
            other.m_Handle = 0;
        }
        return *this;
    }

    void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Handle); }

    void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void Initialize(const void *data, unsigned int size)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
};

} // namespace engine::rendering
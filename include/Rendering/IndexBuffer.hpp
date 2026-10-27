#pragma once
#include <glad/glad.h>

namespace Engine::Rendering
{
    class IndexBuffer
{
private:
    GLuint m_Handle;
    unsigned int m_Count;

public:
    IndexBuffer() : m_Count(0)
    { // careful it requires the indices count
        glGenBuffers(1, &m_Handle);
    }

    ~IndexBuffer() { glDeleteBuffers(1, &m_Handle); }

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept
    {
        m_Handle = other.m_Handle;
        m_Count = other.m_Count;
        other.m_Handle = 0;
        other.m_Count = 0;
    }
    IndexBuffer &operator=(IndexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            if (this->m_Handle != 0)
            {
                glDeleteBuffers(1, &this->m_Handle);
                this->m_Handle = other.m_Handle;
                this->m_Count = other.m_Count;
                other.m_Handle = 0;
                other.m_Count = 0;
            }
        }
        return *this;
    }

    void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle); }

    void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    unsigned int GetCount() const { return m_Count; }

    void Initialize(const unsigned int *data, unsigned int count)
    {
        m_Count = count;
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
};
}
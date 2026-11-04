#pragma once
#include <glad/glad.h>
namespace engine::rendering
{
class VertexBuffer
{

public:
    VertexBuffer() { glGenBuffers(1, &handle_); }

    ~VertexBuffer() { glDeleteBuffers(1, &handle_); }

    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete; // restricts copy

    VertexBuffer(VertexBuffer &&other) noexcept
    {
        handle_ = other.handle_;
        other.handle_ = 0;
    }
    VertexBuffer &operator=(VertexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            glDeleteBuffers(1, &handle_);
            handle_ = other.handle_;
            other.handle_ = 0;
        }
        return *this;
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, handle_); }

    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void initialize(const void *data, unsigned int size)
    {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

private:
    GLuint handle_;
};

} // namespace engine::rendering
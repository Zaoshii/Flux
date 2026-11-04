#pragma once
#include <glad/glad.h>

namespace engine::rendering
{
class VertexArray
{

public:
    VertexArray() { glGenVertexArrays(1, &handle_); }

    ~VertexArray() { glDeleteVertexArrays(1, &handle_); }

    void addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        bind();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
    {
        handle_ = other.handle_;
        other.handle_ = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept
    {
        if (this != &other)
        {
            glDeleteVertexArrays(1, &this->handle_);
            handle_ = other.handle_;
            other.handle_ = 0;
        }
        return *this;
    }

    void bind() const { glBindVertexArray(handle_); }

    void unbind() const { glBindVertexArray(0); }

private:
    GLuint handle_;
};
} // namespace engine::rendering
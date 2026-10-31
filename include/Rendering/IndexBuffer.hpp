#pragma once
#include <glad/glad.h>

namespace engine::rendering
{
class IndexBuffer
{

public:
    IndexBuffer() : count_(0)
    { // careful it requires the indices count
        glGenBuffers(1, &handle_);
    }

    ~IndexBuffer() { glDeleteBuffers(1, &handle_); }

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept
    {
        handle_ = other.handle_;
        count_ = other.count_;
        other.handle_ = 0;
        other.count_ = 0;
    }
    IndexBuffer &operator=(IndexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            if (this->handle_ != 0)
            {
                glDeleteBuffers(1, &this->handle_);
                this->handle_ = other.handle_;
                this->count_ = other.count_;
                other.handle_ = 0;
                other.count_ = 0;
            }
        }
        return *this;
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_); }

    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    unsigned int getCount() const { return count_; }

    void Initialize(const unsigned int *data, unsigned int count)
    {
        count_ = count;
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

private:
    GLuint handle_;
    unsigned int count_;
};
} // namespace engine::rendering
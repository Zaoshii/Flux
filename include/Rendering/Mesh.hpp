#pragma once
#include <glad/glad.h>
#include <vector>
#include "Rendering/index_buffer.hpp"
#include "Rendering/vertex_buffer.hpp"
#include "Rendering/vertex_array.hpp"
#include "Rendering/renderer_types.hpp"
#include "Rendering/Vertex.hpp"
#include <memory>

namespace engine::rendering
{
class Mesh
{

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Primitive type)
        : vertices_(std::move(vertices)), indices_(std::move(indices)),
          primitiveType_(std::move(type))
    {

        vao_.bind();

        vbo_.initialize(this->vertices_.data(),
                        static_cast<unsigned int>(this->vertices_.size() * sizeof(Vertex)));

        ebo_.initialize(this->indices_.data(), static_cast<unsigned int>(this->indices_.size()));
        vao_.unbind();
    }

    ~Mesh() = default;

    void draw()
    {
        vao_.bind();
        glDrawElements(mode(), static_cast<GLsizei>(ebo_.getCount()), GL_UNSIGNED_INT, (void *)0);
    }

    GLenum mode()
    {
        switch (primitiveType_)
        {
        case Primitive::Points:
            return GL_POINTS;
        case Primitive::Lines:
            return GL_LINES;
        case Primitive::LineStrip:
            return GL_LINE_STRIP;
        case Primitive::Triangles:
            return GL_TRIANGLES;
        case Primitive::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case Primitive::TriangleFan:
            return GL_TRIANGLE_FAN;
        default:
            return GL_TRIANGLES;
        }
    }

    void addAttribute(GLuint index, GLint size, GLenum primitiveType, GLboolean normalized,
                      GLsizei stride, const void *pointer)
    {
        vao_.bind();
        vbo_.bind();
        ebo_.bind();
        vao_.addAttribute(index, size, primitiveType, normalized, stride, pointer);
    }

    static std::shared_ptr<Mesh> createCube();

private:
    VertexBuffer vbo_;
    IndexBuffer ebo_;
    VertexArray vao_;

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    Primitive primitiveType_;
};

} // namespace engine::rendering
#pragma once
#include <glad/glad.h>
#include <vector>
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexArray.hpp"
#include "Rendering/RendererTypes.hpp"
#include "Rendering/Vertex.hpp"
#include <memory>

namespace engine::rendering
{
class Mesh
{
private:
    VertexBuffer vbo;
    IndexBuffer ebo;
    VertexArray vao;

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Primitive type;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Primitive type)
        : vertices(std::move(vertices)), indices(std::move(indices)), type(std::move(type))
    {

        vao.Bind();

        vbo.Initialize(this->vertices.data(),
                       static_cast<unsigned int>(this->vertices.size() * sizeof(Vertex)));

        ebo.Initialize(this->indices.data(), static_cast<unsigned int>(this->indices.size()));
        vao.Unbind();
    }

    ~Mesh() = default;

    void Draw()
    {
        vao.Bind();
        glDrawElements(Mode(), static_cast<GLsizei>(ebo.getCount()), GL_UNSIGNED_INT, (void *)0);
    }

    GLenum Mode()
    {
        switch (type)
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

    void AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        vao.Bind();
        vbo.Bind();
        ebo.bind();
        vao.AddAttribute(index, size, type, normalized, stride, pointer);
    }

    static std::shared_ptr<Mesh> CreateCube();
};

} // namespace engine::rendering
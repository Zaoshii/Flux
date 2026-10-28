#pragma once
#include "Rendering/Mesh.hpp"
#include "Rendering/Transform.hpp"
#include <memory>

namespace Engine::Rendering
{

class Cube
{
private:
    std::shared_ptr<Mesh> mesh;

public:
    Cube(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}
    ~Cube() = default;
    Transform transform;
    void Draw() { mesh->Draw(); }
    void AddAttribute(GLuint index, GLint size, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        mesh->AddAttribute(index, size, GL_FLOAT, normalized, stride, pointer);
    };
    glm::mat4 GetModelMatrix() { return transform.GetModelMatrix(); }
};

} // namespace Engine::Rendering
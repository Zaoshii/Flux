#pragma once
#include <glm/glm.hpp>

namespace Engine::Rendering
{
struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    Vertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal)
        : position(position), uv(uv), normal(normal)
    {
    }
};
} // namespace Engine::Rendering
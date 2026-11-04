#pragma once
#include <glm/glm.hpp>

namespace engine::rendering
{
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex(glm::vec3 position_, glm::vec3 normal_, glm::vec2 uv_)
        : position(position_), normal(normal_), uv(uv_)
    {
    }
};
} // namespace engine::rendering
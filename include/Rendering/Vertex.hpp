#pragma once
#include <glm/glm.hpp>

namespace Engine::Rendering
{
    struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;

    Vertex(glm::vec3 position, glm::vec2 uv) : position(position), uv(uv) {}
};
}
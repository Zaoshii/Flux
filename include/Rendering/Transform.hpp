#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine::Rendering
{
struct Transform
{
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    Transform() = default;

    Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    ~Transform() = default;

    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

        return transMat * rotationMat * scaleMat;
    }
};
} // namespace Engine::Rendering

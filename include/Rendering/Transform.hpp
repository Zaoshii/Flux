#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace engine::rendering
{
struct Transform
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform()
        : position(glm::vec3{1.0f}), rotation(glm::quat{1.0f, 0.0f, 0.0f, 0.0f}),
          scale(glm::vec3{1.0f}) {};
    /**
     * @brief Construct a new Transform object
     *
     * @param position_ position
     * @param rotation_ rotation
     * @param scale_ scale
     */
    Transform(glm::vec3 position_, glm::quat rotation_, glm::vec3 scale_)
        : position(position_), rotation(rotation_), scale(scale_)
    {
    }

    ~Transform() = default;

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

        return transMat * rotationMat * scaleMat;
    }
};
} // namespace engine::rendering

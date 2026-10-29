#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine::Rendering
{
class Camera
{
public:
    Camera(glm::vec3 position, glm::quat orientation, float fov, float sensitivity)
        : position(position), orientation(orientation), fov(glm::radians(fov)),
          sensitivity(sensitivity)
    {
    }
    Camera() = default;
    ~Camera() = default;
    glm::vec3 position = glm::vec3{0.0f};
    glm::quat orientation = glm::quat{1.0f, 0.00f, 0.0f, 0.0f};
    float fov = glm::radians(45.0f);
    float sensitivity = 1.0f;
    void Update(float deltaX, float deltaY, float deltaTime);
    glm::mat4 GetViewMatrix();
};
} // namespace Engine::Rendering
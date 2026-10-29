#include "Rendering/Camera.hpp"

namespace Engine::Rendering
{
void Camera::Update(float deltaX, float deltaY, float deltaTime)
{
    float xOffset = deltaX * sensitivity;
    float yOffset = deltaY * sensitivity;

    glm::vec3 cameraRight = orientation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);

    glm::quat rotationPitch = glm::angleAxis(glm::radians(yOffset), cameraRight);
    glm::quat rotationYaw = glm::angleAxis(glm::radians(-xOffset), cameraUp);

    orientation = rotationPitch * rotationYaw * orientation;

    orientation = glm::normalize(orientation);
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 cameraUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = orientation * glm::vec3(.0f, 0.0f, -1.0f); // opengl convention

    return glm::lookAt(position, position + cameraFront, cameraUp);
}
} // namespace Engine::Rendering
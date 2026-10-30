#include "Rendering/Camera.hpp"

namespace engine::rendering
{
void Camera::Update(float deltaMouseX, float deltaMouseY, float deltaTime)
{
    float xOffset = deltaMouseX * sensitivity_;
    float yOffset = deltaMouseY * sensitivity_;

    glm::vec3 cameraRight = orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);

    glm::quat rotationPitch = glm::angleAxis(glm::radians(yOffset), cameraRight);
    glm::quat rotationYaw = glm::angleAxis(glm::radians(-xOffset), cameraUp);

    orientation_ = rotationPitch * rotationYaw * orientation_;

    orientation_ = glm::normalize(orientation_);
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::vec3 cameraUp = orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = orientation_ * glm::vec3(0.0f, 0.0f, -1.0f); // opengl convention

    return glm::lookAt(position_, position_ + cameraFront, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    switch (projectionType_)
    {
    case ProjectionType::Perspective:
        return glm::perspective(fovy_, aspectRatio, nearPlane_, farPlane_);
    case ProjectionType::Orthographic:
        return glm::ortho(-orthographicSize_ * aspectRatio / 2.0f,
                          orthographicSize_ * aspectRatio / 2.0f, -orthographicSize_ / 2.0f,
                          orthographicSize_ / 2.0f, nearPlane_, farPlane_);
    }
    throw std::logic_error("Camera has an invalid projection type");
}
} // namespace engine::rendering
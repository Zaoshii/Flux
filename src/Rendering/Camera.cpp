#include "Rendering/camera.hpp"

namespace engine::rendering
{
void Camera::update(const core::InputManager &inputManager, float deltaTime)
{
    switch (cameraType_)
    {
    case CameraType::FPS:
    {

        float xOffset = inputManager.getDeltaMouseX() * sensitivity_;
        float yOffset = inputManager.getDeltaMouseY() * sensitivity_;

        yaw_ -= xOffset;
        pitch_ += yOffset;

        pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);

        glm::quat yawRotation = glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch_), glm::vec3(1.0f, 0.0f, 0.0f));

        orientation_ = glm::normalize(yawRotation * pitchRotation);

        glm::vec3 cameraRight = glm::normalize(orientation_ * glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraUp = orientation_ * globalUp;
        glm::vec3 cameraForward = glm::normalize(glm::cross(cameraUp, cameraRight));

        if (inputManager.isKeyPressed(GLFW_KEY_W))
        {
            position_ += deltaTime * speed_ * cameraForward;
        }

        if (inputManager.isKeyPressed(GLFW_KEY_S))
        {
            position_ -= deltaTime * speed_ * cameraForward;
        }

        if (inputManager.isKeyPressed(GLFW_KEY_D))
        {
            position_ += deltaTime * speed_ * cameraRight;
        }

        if (inputManager.isKeyPressed(GLFW_KEY_A))
        {
            position_ -= deltaTime * speed_ * cameraRight;
        }

        if (inputManager.isKeyPressed(GLFW_KEY_SPACE))
        {
            position_ += deltaTime * speed_ * globalUp;
        }

        if (inputManager.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        {
            position_ -= deltaTime * speed_ * globalUp;
        }

        orientation_ = glm::normalize(orientation_);

        break;
    }

    case CameraType::Unrestricted:
    {
        float xOffset = inputManager.getDeltaMouseX() * sensitivity_;
        float yOffset = inputManager.getDeltaMouseY() * sensitivity_;

        glm::vec3 localRight = orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 localForward = orientation_ * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 localUp = orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);

        if (localUp.y < 0.0f)
        {
            xOffset = -xOffset;
        }

        glm::quat yawQuat = glm::angleAxis(glm::radians(-xOffset), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchQuat = glm::angleAxis(glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f));

        orientation_ = yawQuat * orientation_ * pitchQuat;

        float currentRoll = 0.0f;
        if (inputManager.isKeyPressed(GLFW_KEY_Q))
        {
            currentRoll -= rollSpeed_ * deltaTime;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_E))
        {
            currentRoll += rollSpeed_ * deltaTime;
        }

        if (currentRoll != 0.0f)
        {
            glm::quat rollQuat =
                glm::angleAxis(glm::radians(currentRoll), glm::vec3(0.0f, 0.0f, -1.0f));
            orientation_ = orientation_ * rollQuat;
        }

        // Always normalize quaternions to prevent floating-point drift
        orientation_ = glm::normalize(orientation_);

        // Movement Translation
        if (inputManager.isKeyPressed(GLFW_KEY_W))
        {
            position_ += deltaTime * speed_ * localForward;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_S))
        {
            position_ -= deltaTime * speed_ * localForward;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_D))
        {
            position_ += deltaTime * speed_ * localRight;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_A))
        {
            position_ -= deltaTime * speed_ * localRight;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_SPACE))
        {
            position_ += deltaTime * speed_ * localUp;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        {
            position_ -= deltaTime * speed_ * localUp;
        }

        break;
    }
    default:
        throw std::logic_error("Camera has an invalid camera type");
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    switch (cameraType_)
    {
    case CameraType::FPS:
    {
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = orientation_ * glm::vec3(0.0f, 0.0f, -1.0f); // opengl convention
        return glm::lookAt(position_, position_ + cameraFront, cameraUp);
    }
    case CameraType::Unrestricted:
    {
        glm::vec3 cameraUp = orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = orientation_ * glm::vec3(0.0f, 0.0f, -1.0f);
        return glm::lookAt(position_, position_ + cameraFront, cameraUp);
    }
    default:
        throw std::logic_error("Camera has an invalid camera type");
        return glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
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
    default:
        throw std::logic_error("Camera has an invalid projection type");
        return glm::perspective(glm::radians(70.0f), 1.777f, 0.1f, 100.0f);
    }
}
} // namespace engine::rendering
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdexcept>
#include <iostream>
#include "camera_settings.hpp"
#include "Core/input_manager.hpp"

namespace engine::rendering
{
/**
 * @brief Represents a camera used to view the 3D scene
 *
 * @details right handed coordinate system, the same as OpenGL
 *
 */
class Camera
{
public:
    /// @brief creates a camera with default configuration
    Camera() = default;

    /**
     * @brief creates a camera with the  specified parameters
     *
     * @note every property is settable
     *
     */
    Camera(const CameraSettings &settings)
        : position_(settings.position), orientation_(settings.orientation),
          projectionType_(settings.projectionType), fovy_(glm::radians(settings.fovy)),
          orthographicSize_(settings.orthographicSize), sensitivity_(settings.sensitivity),
          speed_(settings.speed), rollSpeed_(settings.rollSpeed), nearPlane_(settings.nearPlane),
          farPlane_(settings.farPlane), cameraType_(settings.cameraType)
    {
    }
    ~Camera() = default;

    /**
     * @brief
     *
     * @param deltaMouseX
     * @param deltaMouseY
     * @param deltaTime in seconds
     * @param inputManager for inputs
     */
    void update(const core::InputManager &inputManager, float deltaTime);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    glm::vec3 getPosition() const { return position_; }

    glm::quat getOrientation() const { return orientation_; }

    /**
     * @brief Get the Fovy of the camera
     *
     * @return value in degrees
     */
    float getFovy() const { return glm::degrees(fovy_); }

    /**
     * @brief Set the Fovy of the camera
     *
     * @param value in degrees
     */
    void setFovy(float value) { fovy_ = glm::radians(value); }

    float getOrthographicSize() const { return orthographicSize_; }

    void setOrthographicSize(float value) { orthographicSize_ = value; }

    float getSensitivity() const { return sensitivity_; }

    void setSensitivity(float value) { sensitivity_ = value; }

    ProjectionType getProjectionType() const { return projectionType_; }

    void setProjectionType(ProjectionType value) { projectionType_ = value; }

    float getNearPlane() const { return nearPlane_; }

    void setNearPlane(float value) { nearPlane_ = value; }

    float getFarPlane() const { return farPlane_; }

    void setFarPlane(float value) { farPlane_ = value; }

private:
    glm::vec3 position_;
    glm::quat orientation_;
    float yaw_ = 0.0f;
    float pitch_ = 0.0f;
    ProjectionType projectionType_;
    /// @brief fovy uses degrees externally but radians internally
    float fovy_;
    /// @brief oorthographic size is positive
    float orthographicSize_;
    float sensitivity_;
    float speed_;
    float rollSpeed_;
    float nearPlane_;
    float farPlane_;
    CameraType cameraType_;
};
} // namespace engine::rendering
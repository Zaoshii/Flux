#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdexcept>
#include "camera_settings.hpp"

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
     * @param position initial position of the camera in the world
     * @param orientation initial orientation of the camera in the world
     * @param projectionType the kind of projection the camera will use (settable)
     * @param fovy camera fovy and not fov
     * @param sensitivity sensitivity of the camera
     * @param orthographicSize defines the orthographic size used to calculate the orthographic box
     * for orthographic projection
     * @param nearPlane defines the nearPlane for both orthographic and perspective projection
     * @param farPlane defines the farPlane for both orthographic and perspective projection
     */
    Camera(const CameraSettings &settings)
        : position_(settings.position), orientation_(settings.orientation),
          projectionType_(settings.projectionType), fovy_(glm::radians(settings.fovy)),
          orthographicSize_(settings.orthographicSize), sensitivity_(settings.sensitivity),
          nearPlane_(settings.nearPlane), farPlane_(settings.farPlane)
    {
    }
    ~Camera() = default;

    /**
     * @brief
     *
     * @param deltaMouseX
     * @param deltaMouseY
     * @param deltaTime in seconds
     */
    void Update(float deltaMouseX, float deltaMouseY, float deltaTime);

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
    glm::quat orientation_ = glm::quat{1.0f, 0.00f, 0.0f, 0.0f};
    glm::vec3 position_ = glm::vec3{0.0f};
    ProjectionType projectionType_ = ProjectionType::Perspective;
    /// @brief fovy uses degrees externally but radians internally
    float fovy_ = glm::radians(45.0f);
    /// @brief oorthographic size is positive
    float sensitivity_ = 0.25f;
    float orthographicSize_ = 10.0f;
    float nearPlane_ = 0.1f;
    float farPlane_ = 100.0f;
};
} // namespace engine::rendering
#pragma once

#include <glm/glm.hpp>
#include "renderer_types.hpp"

namespace engine::rendering
{
/**
 * @brief a list of properties used to initialize a camera
 *
 * @ref position is the initial position of the camera in the world
 *
 * @ref orientation is initial orientation of the camera in the world
 *
 * @ref projectionType is the kind of projection the camera will use (settable)
 *
 * @ref fovy is the camera fovy and not fov
 *
 * @ref sensitivity is the camera's sensitivity
 *
 * @ref orthographicSize defines the orthographic size used to calculate the orthographic box for
 * orthographic projection
 *
 * @ref nearPlane defines the nearPlane for both orthographic and perspective projection
 *
 * @ref farPlane defines the farPlane for both orthographic and perspective projection
 */
struct CameraSettings
{
    glm::vec3 position{0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};

    ProjectionType projectionType = ProjectionType::Perspective;

    float fovy = 58.72f; // equivalent of 90 degrees in fov
    float orthographicSize = 10.0f;
    float sensitivity = 0.4f;
    float speed = 20.0f;
    float rollSpeed = 135.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    CameraType cameraType = CameraType::FPS;

    CameraSettings() = default;

    ~CameraSettings() = default;
};
} // namespace engine::rendering
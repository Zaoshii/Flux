#pragma once

#include <glm/glm.hpp>
#include "RendererTypes.hpp"

namespace engine::rendering
{
struct CameraSettings
{
    glm::vec3 position{0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};

    ProjectionType projectionType = ProjectionType::Perspective;

    float fovy = 58.72f; // equivalent of 90 degrees in fov
    float orthographicSize = 10.0f;
    float sensitivity = 0.25f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};
} // namespace engine::rendering
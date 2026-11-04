#pragma once

namespace engine::rendering
{
enum class Primitive
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan
};
enum class ProjectionType
{
    Perspective,
    Orthographic
};
enum class CameraType
{
    Unrestricted,
    FPS
};
} // namespace engine::rendering
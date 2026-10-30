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
} // namespace engine::rendering
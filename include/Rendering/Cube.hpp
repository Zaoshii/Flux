#pragma once
#include "Rendering/Mesh.hpp"
#include "Rendering/Transform.hpp"
#include <memory>

namespace engine::rendering
{
/**
 * @brief represents a cube in the world, holds a shared pointer to a mesh reference
 *
 */
class Cube
{
public:
    Cube(std::shared_ptr<Mesh> mesh) : mesh_(mesh) {}
    ~Cube() = default;
    Transform transform;
    void Draw() const { mesh_->draw(); }
    glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }

private:
    std::shared_ptr<Mesh> mesh_;
};

} // namespace engine::rendering
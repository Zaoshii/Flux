#pragma once
#include "Core/Window.hpp"
#include <memory>

namespace engine::core
{
class InputManager
{
public:
    /**
     * @brief Construct a new Input Manager object and keeps a weak reference to the window it's
     * linked to
     *
     * @param window an engine window which owns the GLFW window
     */
    InputManager(const std::shared_ptr<Window> &window);

    using MouseCallback = std::function<void(float, float)>;

    /**
     * @brief resets mouseDeltas
     *
     */
    void clearDeltas()
    {
        deltaMouseX_ = 0.0f;
        deltaMouseY_ = 0.0f;
    }

    float getMouseX() const { return mouseX_; }

    float getMouseY() const { return mouseY_; }

    float getDeltaMouseX() const { return deltaMouseX_; }

    float getDeltaMouseY() const { return deltaMouseY_; }

private:
    float mouseX_ = 0.0f;
    float mouseY_ = 0.0f;
    float deltaMouseX_ = 0.0f;
    float deltaMouseY_ = 0.0f;
    std::weak_ptr<Window> window_;
    MouseCallback mouseCallback_ = nullptr;

    /**
     * @brief gets called when cursor is moved and sets new mouse position and calculates
     * mouseDeltas
     *
     * @param xpos X component of mouse position given by GLFW
     * @param ypos Y component of mouse position given by GLFW
     */
    void handleMouseMovement(float xpos, float ypos);

    static void glfwMouseCallback(GLFWwindow *window, double xpos, double ypos);
};
} // namespace engine::core
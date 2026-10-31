#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <cstring>
#include <stdexcept>
#include "Core/window_user_data.hpp"

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
    InputManager(const WindowUserData &windowUserData);

    using MouseCallback = std::function<void(float, float)>;

    /**
     * @brief resets mouseDeltas and updates the logic for keys
     *
     */
    void update();

    float getMouseX() const { return mouseX_; }

    float getMouseY() const { return mouseY_; }

    float getDeltaMouseX() const { return deltaMouseX_; }

    float getDeltaMouseY() const { return deltaMouseY_; }

    bool isKeyPressed(int key) const;

    bool wasKeyPressed(int key) const;

    bool wasKeyReleased(int key) const;

private:
    float mouseX_ = 0.0f;
    float mouseY_ = 0.0f;
    bool firstMouse_ = true;
    float deltaMouseX_ = 0.0f;
    float deltaMouseY_ = 0.0f;
    MouseCallback mouseCallback_ = nullptr;
    bool wasPressed_[GLFW_KEY_LAST + 1];
    bool isBeingPressed_[GLFW_KEY_LAST + 1];
    bool wasReleased_[GLFW_KEY_LAST + 1];

    /**
     * @brief gets called when cursor is moved and sets new mouse position and calculates
     * mouseDeltas
     *
     * @param xpos X component of mouse position given by GLFW
     * @param ypos Y component of mouse position given by GLFW
     */
    void handleMouseMovement(float xpos, float ypos);

    static void glfwMouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
} // namespace engine::core
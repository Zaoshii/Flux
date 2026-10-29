#pragma once
#include "Core/Window.hpp"
#include <memory>

namespace Engine::Core
{
class InputManager
{
public:
    InputManager(std::shared_ptr<Window> window);
    using MouseCallback = std::function<void(float, float)>;

    static void glfwMouseCallback(GLFWwindow *window, double xpos, double ypos);

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float deltaMouseX = 0.0f;
    float deltaMouseY = 0.0f;

    void ClearDeltas()
    {
        deltaMouseX = 0.0f;
        deltaMouseY = 0.0f;
    }

private:
    std::weak_ptr<Window> window;
    MouseCallback mouseCallback = nullptr;

    void HandleMouseMovement(float xpos, float ypos);
};
} // namespace Engine::Core
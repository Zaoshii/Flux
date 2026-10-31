#pragma once
#include <GLFW/glfw3.h>

namespace engine::core
{
class Window;
class InputManager;

struct WindowUserData
{
    Window *window = nullptr;
    GLFWwindow *glfwWindow = nullptr;
    InputManager *inputManager = nullptr;
};
} // namespace engine::core
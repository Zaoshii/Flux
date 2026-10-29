#include "Core/InputManager.hpp"

namespace Engine::Core
{
InputManager::InputManager(std::shared_ptr<Window> window) : window(window)
{
    if (window)
    {
        glfwSetWindowUserPointer(window->glfwWindow, this);
        glfwSetCursorPosCallback(window->glfwWindow, glfwMouseCallback);
    }
    else
    {
        throw std::runtime_error("Failed to lock the window pointer in InputManager constructor");
    }
}

void InputManager::glfwMouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    // On récupère l'instance de notre InputManager stockée dans la fenêtre
    auto *instance = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

    if (instance)
    {
        // On envoie les deltas à notre manager
        instance->HandleMouseMovement(xpos, ypos);
    }
}

void InputManager::HandleMouseMovement(float mouseX, float mouseY)
{
    float oldMouseX = this->mouseX;
    float oldMouseY = this->mouseY;
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    deltaMouseX = mouseX - oldMouseX;
    deltaMouseY = oldMouseY - mouseY;
}
} // namespace Engine::Core
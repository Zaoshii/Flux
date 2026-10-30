#include "Core/input_manager.hpp"

namespace engine::core
{
InputManager::InputManager(const std::shared_ptr<Window> &window) : window_(window)
{
    if (window)
    {
        glfwSetWindowUserPointer(window->getGlfwWindow(), this);
        glfwSetCursorPosCallback(window->getGlfwWindow(), glfwMouseCallback);
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
        instance->handleMouseMovement(xpos, ypos);
    }
}

void InputManager::handleMouseMovement(float mouseX, float mouseY)
{
    // doesn't need checking since only called if window alive
    float oldMouseX = this->mouseX_;
    float oldMouseY = this->mouseY_;
    this->mouseX_ = mouseX;
    this->mouseY_ = mouseY;
    deltaMouseX_ = mouseX - oldMouseX;
    deltaMouseY_ = oldMouseY - mouseY;
}
} // namespace engine::core
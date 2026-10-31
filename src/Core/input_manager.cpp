#include "Core/input_manager.hpp"

namespace engine::core
{
InputManager::InputManager(const WindowUserData &windowUserData)
{
    if (windowUserData.window && windowUserData.glfwWindow)
    {
        glfwSetCursorPosCallback(windowUserData.glfwWindow, glfwMouseCallback);
        glfwSetKeyCallback(windowUserData.glfwWindow, glfwKeyCallback);
    }
    else
    {
        throw std::runtime_error("Failed to set GLFW mouse call back");
    }
}

void InputManager::glfwMouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    // On récupère l'instance de notre InputManager stockée dans la fenêtre
    auto *data = static_cast<WindowUserData *>(glfwGetWindowUserPointer(window));

    if (data && data->inputManager)
    {
        data->inputManager->handleMouseMovement(xpos, ypos);
    }
}

void InputManager::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *data = static_cast<WindowUserData *>(glfwGetWindowUserPointer(window));

    if (data && data->inputManager)
    {
        if (key < 0 || key > GLFW_KEY_LAST)
            return;

        if (action == GLFW_PRESS)
        {
            data->inputManager->wasPressed_[key] = true;
            data->inputManager->isBeingPressed_[key] = true;
        }
        else if (action == GLFW_REPEAT)
        {
            return;
        }
        else if (action == GLFW_RELEASE)
        {
            data->inputManager->isBeingPressed_[key] = false;
            data->inputManager->wasReleased_[key] = true;
        }
    }
}

void InputManager::handleMouseMovement(float mouseX, float mouseY)
{
    if (firstMouse_)
    {
        this->mouseX_ = mouseX;
        this->mouseY_ = mouseY;
        firstMouse_ = false;
        return;
    }
    // doesn't need checking since only called if window alive
    float oldMouseX = this->mouseX_;
    float oldMouseY = this->mouseY_;
    this->mouseX_ = mouseX;
    this->mouseY_ = mouseY;
    deltaMouseX_ = mouseX - oldMouseX;
    deltaMouseY_ = oldMouseY - mouseY;
}

void InputManager::update()
{
    deltaMouseX_ = 0.0f;
    deltaMouseY_ = 0.0f;
    std::memset(wasPressed_, 0, sizeof(wasPressed_));
    std::memset(wasReleased_, 0, sizeof(wasReleased_));
}

bool InputManager::isKeyPressed(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        throw std::out_of_range("key check : isKeyPressed doesn't exist ! ");
    }
    return isBeingPressed_[key];
}

bool InputManager::wasKeyPressed(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        throw std::out_of_range("key check : wasKeyPressed doesn't exist ! ");
    }
    return wasPressed_[key];
}

bool InputManager::wasKeyReleased(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        throw std::out_of_range("key check : wasKeyReleased doesn't exist ! ");
    }
    return wasReleased_[key];
}

} // namespace engine::core
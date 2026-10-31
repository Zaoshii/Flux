#include "Core/Window.hpp"
#include <iostream>

namespace engine::core
{

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto *data = static_cast<WindowUserData *>(glfwGetWindowUserPointer(window));
    if (data && data->window)
    {

        data->window->width_ = width;
        data->window->height_ = height;
        data->window->resolution_ = glm::vec2{width, height};
        data->window->aspectRatio_ = (float)width / (float)height;

        glViewport(0, 0, width, height);
    }
}

void Window::swapBuffers()
{
    if (glfwWindow_)
    {
        glfwSwapBuffers(glfwWindow_);
    }
}

void Window::setGLFWContext()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

void Window::setGLFWHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
}

void Window::setWindowParameters()
{
    glfwSetFramebufferSizeCallback(glfwWindow_, framebufferResizeCallback);
    glfwSwapInterval(1); // TODO: Important to know : VSYNC ON
    glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(glfwWindow_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}
} // namespace engine::core
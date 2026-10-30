#include "Core/Window.hpp"

namespace engine::core
{

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    Window *myWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (myWindow)
    {
        myWindow->width_ = width;
        myWindow->height_ = height;
        myWindow->resolution_ = glm::vec2{width, height};
        myWindow->aspectRatio_ = (float)width / (float)height;

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
    glfwSetWindowUserPointer(glfwWindow_, this);
    glfwSetFramebufferSizeCallback(glfwWindow_, framebufferResizeCallback);

    glfwSwapInterval(1); // TODO: Important to know : VSYNC ON
    glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
} // namespace engine::core
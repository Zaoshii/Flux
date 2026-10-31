#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "core/window_user_data.hpp"

namespace engine::core
{
class Window
{
public:
    /**
     * @brief Constructs a new window and sets up GLFW context as well as a few parameters for the
     * window
     *
     */
    Window(int width, int height, std::string title)
        : width_(width), height_(height), resolution_(glm::vec2{width_, height_}),
          aspectRatio_(static_cast<float>(width) / static_cast<float>(height))
    {
        setGLFWContext();

        setGLFWHints();

        glfwWindow_ = glfwCreateWindow(width_, height_, title.c_str(), nullptr, nullptr);

        std::cout << "Window instance: " << this << '\n';
        std::cout << "GLFW handle: " << glfwWindow_ << '\n';

        glfwSetWindowUserPointer(glfwWindow_, this);

        std::cout << "Stored user pointer: " << glfwGetWindowUserPointer(glfwWindow_) << '\n';

        if (!glfwWindow_)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(glfwWindow_);

        setWindowParameters();
    }
    /**
     * @brief Destroys the GLFW context since the window owns it
     *
     */
    ~Window()
    {
        if (glfwWindow_)
        {
            glfwDestroyWindow(glfwWindow_);
            glfwWindow_ = nullptr;
        }
        glfwTerminate();
    }

    void swapBuffers();

    GLFWwindow *getGlfwWindow() const { return glfwWindow_; }

    int getWidth() const { return width_; }

    int getHeight() const { return height_; }

    glm::vec2 getResolution() const { return resolution_; }

    float getAspectRatio() const { return aspectRatio_; }

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

private:
    GLFWwindow *glfwWindow_ = nullptr;
    int width_, height_;
    glm::vec2 resolution_;
    float aspectRatio_;

    void setGLFWContext();

    void setGLFWHints();

    void setWindowParameters();
};
} // namespace engine::core
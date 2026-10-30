#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <stdexcept>

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
    Window()
    {
        setGLFWContext();

        setGLFWHints();

        glfwWindow_ = glfwCreateWindow(width_, height_, "Engine", nullptr, nullptr);

        if (!glfwWindow_)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(glfwWindow_);

        setWindowParameters();
    }

    Window(int width, int height, std::string title)
        : width_(width), height_(height), resolution_(glm::vec2{width_, height_}),
          aspectRatio_(static_cast<float>(width) / static_cast<float>(height))
    {
        setGLFWContext();

        setGLFWHints();

        glfwWindow_ = glfwCreateWindow(width_, height_, title.c_str(), nullptr, nullptr);

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

private:
    GLFWwindow *glfwWindow_ = nullptr;
    int width_ = 800, height_ = 600;
    glm::vec2 resolution_ = glm::vec2{width_, height_};
    float aspectRatio_ = (float)width_ / (float)height_;

    void setGLFWContext();

    void setGLFWHints();

    void setWindowParameters();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
};
} // namespace engine::core
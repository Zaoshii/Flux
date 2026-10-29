#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <stdexcept>

namespace Engine::Core
{
class Window
{
public:
    GLFWwindow *glfwWindow = nullptr;
    int WIDTH = 800, HEIGHT = 600;
    glm::vec2 RESOLUTION = glm::vec2{WIDTH, HEIGHT};
    float ASPECTRATIO = (float)WIDTH / (float)HEIGHT;

    Window()
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "Engine", nullptr, nullptr);

        if (!glfwWindow)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(glfwWindow);
        glfwSetWindowUserPointer(glfwWindow, this);

        glfwSetFramebufferSizeCallback(glfwWindow, FramebufferResizeCallback);

        glfwSwapInterval(1); // TODO: Important to know : VSYNC ON
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Window(int width, int height, std::string title)
        : WIDTH(width), HEIGHT(height),
          ASPECTRATIO(static_cast<float>(width) / static_cast<float>(height))
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);

        if (!glfwWindow)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(glfwWindow);
        glfwSetWindowUserPointer(glfwWindow, this);

        glfwSetFramebufferSizeCallback(glfwWindow, FramebufferResizeCallback);

        glfwSwapInterval(1); // TODO: Important to know : VSYNC ON
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    ~Window()
    {
        if (glfwWindow)
        {
            glfwDestroyWindow(glfwWindow);
            glfwWindow = nullptr;
        }
        glfwTerminate();
    }

    static void FramebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        Window *myWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (myWindow)
        {
            myWindow->WIDTH = width;
            myWindow->HEIGHT = height;
            myWindow->RESOLUTION = glm::vec2{width, height};
            myWindow->ASPECTRATIO = (float)width / (float)height;

            glViewport(0, 0, width, height);
        }
    }

    void SwapBuffers()
    {
        if (glfwWindow)
        {
            glfwSwapBuffers(glfwWindow);
        }
    }
};
} // namespace Engine::Core
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "Core/Timer.hpp"
namespace Engine
{
    struct Engine
{
    GLFWwindow *window = nullptr;
    int WIDTH = 800, HEIGHT = 600;
    glm::vec2 RESOLUTION{WIDTH, HEIGHT};
    Core::Timer *timer = nullptr; // optional timer hook

    Engine()
    {
        glfwSetErrorCallback(
            [](int error, const char *description)
            { std::cerr << "GLFW Error " << error << ": " << description << '\n'; });

        if (!glfwInit())
        {
            std::cerr << "failed to initialize GLFW\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Skeleton Body", nullptr, nullptr);

        if (!window)
        {
            std::cerr << "Failed to initialize GLFW window\n";
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);

        glfwSwapInterval(1); // Vsync On

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cerr << "Failed to initialize Glad\n";
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        glEnable(GL_MULTISAMPLE);

        glfwSetWindowUserPointer(window, this);

        glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

        std::cout << "OpenGL version is : " << glGetString(GL_VERSION) << '\n';
    }

    ~Engine()
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void Run()
    {
        glClearColor(53.0 / 255.0, 19.0 / 255.0, 79.0 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    static void FramebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        if (engine)
        {
            engine->WIDTH = width; // This is now true physical pixels
            engine->HEIGHT = height;
            engine->RESOLUTION = glm::vec2{width, height};

            glViewport(0, 0, width, height);
        }
    }
};
}
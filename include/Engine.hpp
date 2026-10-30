#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "Core/Timer.hpp"
#include "Core/Window.hpp"
#include <memory>
namespace engine
{
struct Engine
{
    std::shared_ptr<core::Window> window =
        nullptr; // optional window hook to allow for custom window management
    std::unique_ptr<core::Timer> timer = nullptr; // optional timer hook

    Engine()
    {
        window = std::make_shared<core::Window>(800, 600, "Engine");

        glfwSetErrorCallback(
            [](int error, const char *description)
            { std::cerr << "GLFW Error " << error << ": " << description << '\n'; });

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cerr << "Failed to initialize Glad\n";
            glfwDestroyWindow(window->getGlfwWindow());
            glfwTerminate();
        }

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Rejeter les faces arrière (comportement par défaut)
        glFrontFace(GL_CCW); // Définir que le sens anti-horaire (Counter-ClockWise) est l'avant du
        //     triangle
        glEnable(GL_DEPTH_TEST); // TODO: important counter-clockwise is front face, clockwise is
                                 // back face

        std::cout << "OpenGL version is : " << glGetString(GL_VERSION) << '\n';
    }

    Engine(int width, int height, std::string title)
    {
        window = std::make_shared<core::Window>(width, height, title);

        glfwSetErrorCallback(
            [](int error, const char *description)
            { std::cerr << "GLFW Error " << error << ": " << description << '\n'; });

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cerr << "Failed to initialize Glad\n";
            glfwDestroyWindow(window->getGlfwWindow());
            glfwTerminate();
        }

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Rejeter les faces arrière (comportement par défaut)
        glFrontFace(GL_CCW); // Définir que le sens anti-horaire (Counter-ClockWise) est l'avant du
        //     triangle
        glEnable(GL_DEPTH_TEST); // TODO: important counter-clockwise is front face, clockwise is
                                 // back face

        std::cout << "OpenGL version is : " << glGetString(GL_VERSION) << '\n';
    }

    ~Engine() = default;

    void Run()
    {
        glClearColor(53.0 / 255.0, 19.0 / 255.0, 79.0 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
} // namespace engine
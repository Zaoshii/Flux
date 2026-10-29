#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include "Engine.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Cube.hpp"
#include "Core/InputManager.hpp"
#include "Rendering/Camera.hpp"

#include <numbers>
#include <mdspan>

constexpr float PI = std::numbers::pi;

// force buil

void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar *message, const void *userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::cerr << "[OPENGL ERROR] " << message << '\n';
    }
    else if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        std::cerr << "[OPENGL WARNING] " << message << '\n';
    }
}

int main()
{

    Engine::Engine engine(1920, 1080, "quaternion Camera waouh!");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);

    Engine::Core::Timer timer;
    engine.timer = std::make_unique<Engine::Core::Timer>(timer);

    float deltaTime = 0.0f;

    int frameCount = 0;

    float rotationSpeed = 0.25f;
    float rotationSpeedX = 1.6f;
    float rotationSpeedY = 1.0f;
    float rotationSpeedZ = 1.4f;

    glm::vec3 X(1.0f, 0.0f, 0.0f);
    glm::vec3 Y(0.0f, 1.0f, 0.0f);
    glm::vec3 Z(0.0f, 0.0f, 1.0f);

    constexpr size_t width = 5;
    constexpr size_t height = 5;
    constexpr size_t depth = 5;

    Engine::Rendering::Camera camera(glm::vec3((float)(width) * 2.5f + 5.0f, (float)(height) * 1.5f,
                                               (float)(depth) * 2.5f + 5.0f),
                                     glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 360.0f, 0.25f);

    glm::mat4 fixedProjection =
        glm::perspective(camera.fov, engine.window->ASPECTRATIO, 0.1f, 100.0f);

    std::shared_ptr<Engine::Rendering::Mesh> cubeMesh = Engine::Rendering::Mesh::CreateCube();
    cubeMesh->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Rendering::Vertex), (void *)0);
    cubeMesh->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Rendering::Vertex),
                           (void *)(3 * sizeof(GL_FLOAT)));
    cubeMesh->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Rendering::Vertex),
                           (void *)(5 * sizeof(GL_FLOAT)));

    Engine::Rendering::Shader cubeShader("resources/shaders/cube.vert",
                                         "resources/shaders/cube.frag");

    std::vector<Engine::Rendering::Cube> rawCubes(width * height * depth,
                                                  Engine::Rendering::Cube(cubeMesh));
    auto cubeGrid = std::mdspan(rawCubes.data(), width, height, depth);

    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            for (size_t z = 0; z < depth; z++)
            {
                cubeGrid[x, y, z].transform.position.x = 2 * x;
                cubeGrid[x, y, z].transform.position.y = 2 * y;
                cubeGrid[x, y, z].transform.position.z = 2 * z;
            }
        }
    }

    Engine::Core::InputManager inputManager(engine.window);

    while (!glfwWindowShouldClose(engine.window->glfwWindow))
    { // Main loop
        glfwPollEvents();

        deltaTime = timer.DeltaTime();
        if (frameCount % 50 == 0)
        {
            std::cout << std::format("The frame rate is : {}\n", 1.0f / deltaTime);
            /*std::cout << inputManager.mouseX << "\n"
                      << inputManager.mouseY << "\n"
                      << inputManager.deltaMouseX << "\n"
                      << inputManager.deltaMouseY << "\n";*/
        }
        fixedProjection = glm::perspective(camera.fov, engine.window->ASPECTRATIO, 0.1f, 100.0f);
        float angleRotationThisFrame = rotationSpeed * deltaTime;
        glm::quat rotX = glm::angleAxis(angleRotationThisFrame * rotationSpeedX, X);
        glm::quat rotY = glm::angleAxis(angleRotationThisFrame * rotationSpeedY, Y);
        glm::quat rotZ = glm::angleAxis(angleRotationThisFrame * rotationSpeedZ, Z);
        glm::quat rotFinal = rotX * rotY * rotZ;
        engine.Run();
        cubeShader.Use();
        cubeShader.SetMat4("uView", camera.GetViewMatrix());
        cubeShader.SetMat4("uProj", fixedProjection);
        camera.Update(inputManager.deltaMouseX, inputManager.deltaMouseY, deltaTime);
        for (auto &cube : rawCubes)
        {
            cube.transform.rotation = rotFinal * cube.transform.rotation;
            cube.transform.rotation = glm::normalize(cube.transform.rotation);
            cubeShader.SetMat4("uModel", cube.transform.GetModelMatrix());
            cube.Draw();
        }

        engine.window->SwapBuffers();
        frameCount++;
        inputManager.ClearDeltas();
    }

    return 0;
}
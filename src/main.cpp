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
#include "Core/input_manager.hpp"
#include "Rendering/Camera.hpp"

#include <numbers>
#include <mdspan>

constexpr float PI = std::numbers::pi;

// force buil

using namespace engine;

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

    Engine engine(1500, 1000, "quaternion Camera waouh!");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);

    core::Timer timer;
    engine.timer = std::make_unique<core::Timer>(timer);

    float deltaTime = 0.0f;

    int frameCount = 0;

    float rotationSpeed = 0.25f;
    float rotationSpeedX = 1.6f;
    float rotationSpeedY = 1.0f;
    float rotationSpeedZ = 1.4f;

    glm::vec3 X(1.0f, 0.0f, 0.0f);
    glm::vec3 Y(0.0f, 1.0f, 0.0f);
    glm::vec3 Z(0.0f, 0.0f, 1.0f);

    constexpr size_t width = 8;
    constexpr size_t height = 8;
    constexpr size_t depth = 8;

    rendering::CameraSettings settings;
    settings.position = glm::vec3((float)(width) * 4.5f + 5.0f, (float)(height) * 1.5f,
                                  (float)(depth) * 4.5f + 5.0f);
    settings.projectionType = rendering::ProjectionType::Orthographic;
    settings.orthographicSize = 50.0f;

    rendering::Camera camera(settings);

    std::shared_ptr<rendering::Mesh> cubeMesh = rendering::Mesh::CreateCube();
    cubeMesh->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex), (void *)0);
    cubeMesh->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                           (void *)(3 * sizeof(GL_FLOAT)));
    cubeMesh->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                           (void *)(5 * sizeof(GL_FLOAT)));

    rendering::Shader cubeShader("resources/shaders/cube.vert", "resources/shaders/cube.frag");

    std::vector<rendering::Cube> rawCubes(width * height * depth, rendering::Cube(cubeMesh));
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

    core::InputManager inputManager(engine.window);

    while (!glfwWindowShouldClose(engine.window->getGlfwWindow()))
    { // Main loop
        glfwPollEvents();
        timer.update();
        deltaTime = timer.getDeltaTime();
        if (frameCount % 50 == 0)
        {
            std::cout << std::format("The frame rate is : {}\n", 1.0f / deltaTime);
            /*std::cout << inputManager.mouseX << "\n"
                      << inputManager.mouseY << "\n"
                      << inputManager.deltaMouseX << "\n"
                      << inputManager.deltaMouseY << "\n";*/
        }
        float angleRotationThisFrame = rotationSpeed * deltaTime;
        glm::quat rotX = glm::angleAxis(angleRotationThisFrame * rotationSpeedX, X);
        glm::quat rotY = glm::angleAxis(angleRotationThisFrame * rotationSpeedY, Y);
        glm::quat rotZ = glm::angleAxis(angleRotationThisFrame * rotationSpeedZ, Z);
        glm::quat rotFinal = rotX * rotY * rotZ;
        engine.Run();
        cubeShader.Use();
        cubeShader.SetMat4("uView", camera.getViewMatrix());
        cubeShader.SetMat4("uProj", camera.getProjectionMatrix(engine.window->getAspectRatio()));
        camera.Update(inputManager.getDeltaMouseX(), inputManager.getDeltaMouseY(), deltaTime);
        for (auto &cube : rawCubes)
        {
            cube.transform.rotation = rotFinal * cube.transform.rotation;
            cube.transform.rotation = glm::normalize(cube.transform.rotation);
            cubeShader.SetMat4("uModel", cube.transform.GetModelMatrix());
            cube.Draw();
        }

        engine.window->swapBuffers();
        frameCount++;
        inputManager.clearDeltas();
    }

    return 0;
}
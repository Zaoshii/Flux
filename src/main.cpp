#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Engine.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Cube.hpp"
#include "Core/input_manager.hpp"
#include "Rendering/camera.hpp"

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

    Engine engine(1280, 720, "quaternion Camera waouh!");

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << '\n';

    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    std::cout << "Main object: " << engine.window.get()
              << " | Aspect: " << engine.window->getAspectRatio() << '\n';

    std::cout << "Engine window object: " << engine.window.get() << '\n';

    std::cout << "Engine GLFW handle: " << engine.window->getGlfwWindow() << '\n';
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
    settings.projectionType = rendering::ProjectionType::Perspective;
    settings.cameraType = rendering::CameraType::Unrestricted;
    settings.orthographicSize = 30.0f;
    settings.farPlane = 1000.0f;

    rendering::Camera camera(settings);

    std::shared_ptr<rendering::Mesh> cubeMesh = rendering::Mesh::CreateCube();
    cubeMesh->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex), (void *)0);
    cubeMesh->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                           (void *)(3 * sizeof(GL_FLOAT)));
    cubeMesh->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                           (void *)(5 * sizeof(GL_FLOAT)));

    rendering::Cube cubeSky(cubeMesh);
    rendering::Shader cubeSkyShader("resources/shaders/cube_sky.vert",
                                    "resources/shaders/cube_sky.frag");
    cubeSky.transform.scale = glm::vec3(200.0f, 200.0f, 200.0f);

    rendering::Cube cubeFloor(cubeMesh);
    rendering::Shader cubeFloorShader("resources/shaders/cube_floor.vert",
                                      "resources/shaders/cube_floor.frag");
    cubeFloor.transform.position = glm::vec3(0.0f, -150.0f, 0.0f);
    cubeFloor.transform.scale = glm::vec3(100.0f, 100.0f, 100.0f);

    rendering::Shader cubeShader("resources/shaders/cube.vert", "resources/shaders/cube.frag");

    std::vector<rendering::Cube> rawCubes(width * height * depth, rendering::Cube(cubeMesh));
    auto cubeGrid = std::mdspan(rawCubes.data(), width, height, depth);
    rendering::Cube bigCube(cubeMesh);
    bigCube.transform.position = glm::vec3(-5.0f, 0.0f, -5.0f);
    bigCube.transform.scale = glm::vec3(3.0f, 3.0f, 3.0f);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(engine.window->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    // Frame buffer experimentation

    GLuint framebuffer;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint colorTexture;

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    GLuint depthBuffer;

    glGenRenderbuffers(1, &depthBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              depthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is incomplete!" << std::endl;
    }

    rendering::Mesh screenQuad(
        std::vector<rendering::Vertex>{
            rendering::Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
            rendering::Vertex{{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
            rendering::Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
            rendering::Vertex{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}},
        std::vector<unsigned int>{0u, 1u, 2u, 1u, 3u, 2u}, rendering::Primitive::Triangles);

    rendering::Shader screenShader("resources/shaders/bayer_dithering.vert",
                                   "resources/shaders/bayer_dithering.frag");

    screenQuad.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex), (void *)0);
    screenQuad.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                            (void *)(3 * sizeof(GL_FLOAT)));
    screenQuad.AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(rendering::Vertex),
                            (void *)(5 * sizeof(GL_FLOAT)));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(engine.window->getGlfwWindow()))
    { // Main loop
        engine.inputManager->update();
        glfwPollEvents();
        if (engine.inputManager->wasKeyPressed(GLFW_KEY_L))
        {
            glfwSetInputMode(engine.window->getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        }
        if (engine.inputManager->wasKeyPressed(GLFW_KEY_K))
        {
            glfwSetInputMode(engine.window->getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        timer.update();
        deltaTime = timer.getDeltaTime();
        if (frameCount % 50 == 0)
        {
            std::cout << std::format("The frame rate is : {}\n", 1.0f / deltaTime);
        }
        float angleRotationThisFrame = rotationSpeed * deltaTime;
        glm::quat rotX = glm::angleAxis(angleRotationThisFrame * rotationSpeedX, X);
        glm::quat rotY = glm::angleAxis(angleRotationThisFrame * rotationSpeedY, Y);
        glm::quat rotZ = glm::angleAxis(angleRotationThisFrame * rotationSpeedZ, Z);
        glm::quat rotFinal = rotX * rotY * rotZ;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        cubeSkyShader.use();
        cubeSkyShader.setMat4("uModel", cubeSky.transform.getModelMatrix());
        cubeSkyShader.setMat4("uView", camera.getViewMatrix());
        cubeSkyShader.setMat4("uProj", camera.getProjectionMatrix(engine.window->getAspectRatio()));
        glDisable(GL_CULL_FACE);
        cubeSky.Draw();
        glEnable(GL_CULL_FACE);

        cubeShader.use();
        camera.update(*engine.inputManager, deltaTime);
        cubeShader.setMat4("uView", camera.getViewMatrix());
        // std::cout << "Aspect ratio: " << engine.window->getAspectRatio() << '\n';
        cubeShader.setMat4("uProj", camera.getProjectionMatrix(engine.window->getAspectRatio()));
        for (auto &cube : rawCubes)
        {
            cube.transform.rotation = rotFinal * cube.transform.rotation;
            cube.transform.rotation = glm::normalize(cube.transform.rotation);
            cubeShader.setMat4("uModel", cube.transform.getModelMatrix());
            cube.Draw();
        }
        cubeShader.setMat4("uModel", bigCube.getModelMatrix());
        bigCube.Draw();

        cubeFloorShader.use();
        cubeFloorShader.setMat4("uModel", cubeFloor.transform.getModelMatrix());
        cubeFloorShader.setMat4("uView", camera.getViewMatrix());
        cubeFloorShader.setMat4("uProj",
                                camera.getProjectionMatrix(engine.window->getAspectRatio()));
        cubeFloor.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        screenShader.use();
        screenShader.setInt("screenTexture", 0);
        screenQuad.Draw();

        engine.window->swapBuffers();
        frameCount++;
        glEnable(GL_DEPTH_TEST);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
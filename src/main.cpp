#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "Engine.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"

#include <numbers>

constexpr float PI = std::numbers::pi;

// force build

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

class Ellipse : public Engine::Rendering::Mesh
{
public:
    Ellipse(glm::vec2 p1, glm::vec2 p2, int segments)
        : Mesh(GenerateVertices(p1, p2, segments), GenerateIndices(segments),
               Engine::Rendering::Primitive::TriangleFan)
    {
        AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Rendering::Vertex), (void *)0);
        AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Rendering::Vertex), (void *)(3 * sizeof(GLfloat)));
    }

private:
    std::vector<Engine::Rendering::Vertex> GenerateVertices(glm::vec2 p1, glm::vec2 p2, int segments)
    {
        std::vector<Engine::Rendering::Vertex> vertices;
        glm::vec2 center{(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f};
        float rx = std::abs(p2.x - p1.x) / 2.0f;
        float ry = std::abs(p2.y - p1.y) / 2.0f;

        vertices.push_back(Engine::Rendering::Vertex{{center.x, center.y, 0.0f}, {0.5f, 0.5f}}); // center

        float angleStep = (2.0f * PI) / static_cast<float>(segments);

        for (int i = 0; i < segments; i++)
        {

            float theta = i * angleStep;

            float x = center.x + std::cosf(theta) * rx;
            float y = center.y + std::sinf(theta) * ry;

            float u = std::cosf(theta) / 2.0f + 0.5f; // mapping from [-1;1] to [0;1]
            float v = std::sinf(theta) / 2.0f + 0.5f;

            vertices.push_back(Engine::Rendering::Vertex{{x, y, 0.0f}, {u, v}});
        }
        return vertices;
    }

    std::vector<unsigned int> GenerateIndices(int segments)
    {
        std::vector<unsigned int> indices;
        for (int i = 0; i <= segments; ++i)
        {
            indices.push_back(i);
        }
        indices.push_back(1);
        return indices;
    }
};

int main()
{

    Engine::Engine engine;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);

    Engine::Core::Timer timer;
    engine.timer = &timer;

    float deltaTime = 0.0f;

    float hyp1 = std::sqrtf(2.0f * std::powf(0.5f, 2.0f));
    float hyp2 = std::sqrtf(2.0f * std::powf(0.3f, 2.0f));
    float hyp3 = std::sqrtf(2.0f * std::powf(0.15f, 2.0f));

    float offset1 = 0;
    float offset2 = -0.55;
    float offset3 = +0.75f;

    Ellipse ellipse1(glm::vec2{-hyp1, -hyp1}, glm::vec2(hyp1, hyp1), 64);
    Ellipse ellipse2(glm::vec2{-hyp2 + offset2, -hyp2 + offset2},
                     glm::vec2(hyp2 + offset2, hyp2 + offset2), 64);
    Ellipse ellipse3(glm::vec2{-hyp3 + offset3, -hyp3 + offset3},
                     glm::vec2(hyp3 + offset3, hyp3 + offset3), 64);

    Engine::Rendering::Shader ellipseShader("resources/shaders/ellipse.vert", "resources/shaders/ellipse.frag");

    int frameCount = 0;

    while (!glfwWindowShouldClose(engine.window))
    { // Main loop
        glfwPollEvents();

        deltaTime = timer.DeltaTime();
        if (frameCount % 50 == 0)
            std::cout << std::format("The frame rate is : {}\n", 1.0f / deltaTime);
        engine.Run();
        ellipseShader.Use();
        ellipseShader.SetFloat("uTime", timer.TotalTime());
        ellipseShader.SetVec2("uResolution", engine.RESOLUTION);
        ellipse1.Draw();
        ellipse2.Draw();
        ellipse3.Draw();

        glfwSwapBuffers(engine.window);
        frameCount++;
    }

    return 0;
}
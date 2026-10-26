#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numbers>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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

std::string GetShaderContent(std::string path)
{
    std::ifstream input(path, std::ios::in);

    if (!input.is_open() || !input.good())
        throw std::runtime_error("Could not open the specified path");

    std::stringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

enum class Primitive
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan
};

class Timer
{
private:
    std::chrono::steady_clock::time_point m_StartPoint;
    std::chrono::steady_clock::time_point m_LastPoint;

public:
    Timer() { Reset(); }

    ~Timer() = default;

    void Reset()
    {
        m_StartPoint = std::chrono::steady_clock::now();
        m_LastPoint = m_StartPoint;
    }

    float DeltaTime()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration<float>(now - m_LastPoint);
        m_LastPoint = now;
        float dt = elapsedTime.count();
        if (dt > 0.05f)
            dt = 0.006f; // if dt too long consider basic 165 fps frame
        return dt;
    }

    float TotalTime()
    {
        auto now = std::chrono::steady_clock::now();
        auto totalTime = std::chrono::duration<float>(now - m_StartPoint);
        return totalTime.count();
    }
};

struct Engine
{
    GLFWwindow *window = nullptr;
    int WIDTH = 800, HEIGHT = 600;
    glm::vec2 RESOLUTION{WIDTH, HEIGHT};
    Timer *timer = nullptr; // optional timer hook

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

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;

    Vertex(glm::vec3 position, glm::vec2 uv) : position(position), uv(uv) {}
};

class VertexBuffer
{
private:
    GLuint m_Handle;

public:
    VertexBuffer() { glGenBuffers(1, &m_Handle); }

    ~VertexBuffer() { glDeleteBuffers(1, &m_Handle); }

    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete; // empêche de copier

    VertexBuffer(VertexBuffer &&other) noexcept
    {
        m_Handle = other.m_Handle;
        other.m_Handle = 0;
    }
    VertexBuffer &operator=(VertexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            if (m_Handle != 0)
            {
                glDeleteBuffers(1, &m_Handle);
            }
            m_Handle = other.m_Handle;
            other.m_Handle = 0;
        }
        return *this;
    }

    void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Handle); }

    void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void Initialize(const void *data, unsigned int size)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
};

class IndexBuffer
{
private:
    GLuint m_Handle;
    unsigned int m_Count;

public:
    IndexBuffer() : m_Count(0)
    { // careful it requires the indices count
        glGenBuffers(1, &m_Handle);
    }

    ~IndexBuffer() { glDeleteBuffers(1, &m_Handle); }

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept
    {
        m_Handle = other.m_Handle;
        m_Count = other.m_Count;
        other.m_Handle = 0;
        other.m_Count = 0;
    }
    IndexBuffer &operator=(IndexBuffer &&other) noexcept
    {
        if (this != &other)
        {
            if (this->m_Handle != 0)
            {
                glDeleteBuffers(1, &this->m_Handle);
                this->m_Handle = other.m_Handle;
                this->m_Count = other.m_Count;
                other.m_Handle = 0;
                other.m_Count = 0;
            }
        }
        return *this;
    }

    void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle); }

    void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    unsigned int GetCount() const { return m_Count; }

    void Initialize(const unsigned int *data, unsigned int count)
    {
        m_Count = count;
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
};

class VertexArray
{
private:
    GLuint m_Handle;

public:
    VertexArray() { glGenVertexArrays(1, &m_Handle); }

    ~VertexArray() { glDeleteVertexArrays(1, &m_Handle); }

    void AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        Bind();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
    {
        m_Handle = other.m_Handle;
        other.m_Handle = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept
    {
        if (this != &other)
        {
            if (this->m_Handle != 0)
            {
                glDeleteVertexArrays(1, &this->m_Handle);
            }
            m_Handle = other.m_Handle;
            other.m_Handle = 0;
        }
        return *this;
    }

    void Bind() const { glBindVertexArray(m_Handle); }

    void Unbind() const { glBindVertexArray(0); }
};

class Shader
{
private:
    GLuint m_Handle;
    mutable std::unordered_map<std::string, GLint> uniformLocationCache;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath)
    {

        int success;
        char infoLog[512];

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        std::string vertexSource = GetShaderContent(vertexPath);
        std::string fragmentSource = GetShaderContent(fragmentPath);

        const char *vertexCode = vertexSource.c_str();
        const char *fragmentCode = fragmentSource.c_str();

        glShaderSource(vertexShader, 1, &vertexCode, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog));
        }

        glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog));
        }

        m_Handle = glCreateProgram();

        glAttachShader(m_Handle, vertexShader);
        glAttachShader(m_Handle, fragmentShader);
        glLinkProgram(m_Handle);
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_Handle, 512, nullptr, infoLog);
            throw std::runtime_error(
                std::format("ERROR::SHADER::PROGRAM::LINKING::FAILED\n{}", infoLog));
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() { glDeleteProgram(m_Handle); }

    void Use() { glUseProgram(m_Handle); }

    GLuint GetID() { return m_Handle; }

    GLint GetUniformLocation(const std::string &name) const
    {
        if (auto iterator = uniformLocationCache.find(name); iterator != uniformLocationCache.end())
        {
            return iterator->second;
        }
        GLint location = glGetUniformLocation(m_Handle, name.c_str()); // lazy caching
        uniformLocationCache[name] = location;
        return location;
    }

    void SetFloat(const std::string &name, const float data) const
    {
        glUniform1f(GetUniformLocation(name), data);
    }

    void SetVec2(const std::string &name, const glm::vec2 data) const
    {
        glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(data));
    }
};

class Mesh
{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Primitive type;

    VertexBuffer vbo;
    IndexBuffer ebo;

public:
    VertexArray vao;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Primitive type)
        : vertices(std::move(vertices)), indices(std::move(indices)), type(std::move(type))
    {

        vao.Bind();

        vbo.Initialize(this->vertices.data(),
                       static_cast<unsigned int>(this->vertices.size() * sizeof(Vertex)));

        ebo.Initialize(this->indices.data(), static_cast<unsigned int>(this->indices.size()));
        vao.Unbind();
    }

    void Draw()
    {
        vao.Bind();
        glDrawElements(Mode(), static_cast<GLsizei>(ebo.GetCount()), GL_UNSIGNED_INT, (void *)0);
    }

    GLenum Mode()
    {
        switch (type)
        {
        case Primitive::Points:
            return GL_POINTS;
        case Primitive::Lines:
            return GL_LINES;
        case Primitive::LineStrip:
            return GL_LINE_STRIP;
        case Primitive::Triangles:
            return GL_TRIANGLES;
        case Primitive::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case Primitive::TriangleFan:
            return GL_TRIANGLE_FAN;
        default:
            return GL_TRIANGLES;
        }
    }

    void AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                      const void *pointer)
    {
        vao.Bind();
        vbo.Bind();
        ebo.Bind();
        vao.AddAttribute(index, size, type, normalized, stride, pointer);
    }
};

class Ellipse : public Mesh
{
public:
    Ellipse(glm::vec2 p1, glm::vec2 p2, int segments)
        : Mesh(GenerateVertices(p1, p2, segments), GenerateIndices(segments),
               Primitive::TriangleFan)
    {
        AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
    }

private:
    std::vector<Vertex> GenerateVertices(glm::vec2 p1, glm::vec2 p2, int segments)
    {
        std::vector<Vertex> vertices;
        glm::vec2 center{(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f};
        float rx = std::abs(p2.x - p1.x) / 2.0f;
        float ry = std::abs(p2.y - p1.y) / 2.0f;

        vertices.push_back(Vertex{{center.x, center.y, 0.0f}, {0.5f, 0.5f}}); // center

        float angleStep = (2.0f * PI) / static_cast<float>(segments);

        for (int i = 0; i < segments; i++)
        {

            float theta = i * angleStep;

            float x = center.x + std::cosf(theta) * rx;
            float y = center.y + std::sinf(theta) * ry;

            float u = std::cosf(theta) / 2.0f + 0.5f; // mapping from [-1;1] to [0;1]
            float v = std::sinf(theta) / 2.0f + 0.5f;

            vertices.push_back(Vertex{{x, y, 0.0f}, {u, v}});
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

struct Transform2D
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    Transform2D(glm::vec2 position, float rotation, glm::vec2 scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    ~Transform2D() = default;

    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale, 1.0f));
        return model;
    }
};

int main()
{

    Engine engine;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);

    Timer timer;
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

    Shader ellipseShader("resources/shaders/ellipse.vert", "resources/shaders/ellipse.frag");

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
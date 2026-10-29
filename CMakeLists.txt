cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(Engine
    src/main.cpp
    include/Engine.hpp
    include/Rendering/VertexArray.hpp
    include/Rendering/IndexBuffer.hpp
    include/Rendering/VertexBuffer.hpp
    include/Rendering/Shader.hpp
    include/Core/Timer.hpp
    include/Rendering/Mesh.hpp
    include/Rendering/RendererTypes.hpp
    src/Engine.cpp
    src/Core/Timer.cpp
    include/Rendering/Camera.hpp
    src/Rendering/Camera.cpp
    src/Rendering/Shader.cpp
    include/Rendering/Vertex.hpp
    include/Rendering/Cube.hpp
    src/Rendering/Cube.cpp
    include/Rendering/Transform.hpp
    src/Rendering/Mesh.cpp
    include/Core/Log.hpp
    include/Core/Window.hpp
    include/Core/InputManager.hpp
    src/Core/InputManager.cpp
)

target_include_directories(Engine PRIVATE include)

target_link_libraries(Engine PRIVATE
    glfw
    glm::glm
    glad::glad
    OpenGL::GL
)

# Automatically copy the resources folder to the build directory after every build
add_custom_command(
    TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Engine>/resources"
)

set_property(TARGET Engine PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "C:/Users/Z230/Desktop/Code/C++"
)
cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

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
    include/Rendering/camera.hpp
    src/Rendering/camera.cpp
    src/Rendering/Shader.cpp
    include/Rendering/Vertex.hpp
    include/Rendering/Cube.hpp
    src/Rendering/Cube.cpp
    include/Rendering/Transform.hpp
    src/Rendering/Mesh.cpp
    include/Core/Log.hpp
    include/Core/Window.hpp
    include/Core/input_manager.hpp
    src/Core/input_manager.cpp
    src/Core/window.cpp
    include/Rendering/camera_settings.hpp
    include/Core/file/file.hpp
    src/Core/file/file.cpp
    include/Core/window_user_data.hpp
)

target_include_directories(Engine PRIVATE include)

target_link_libraries(Engine PRIVATE
    glfw
    glm::glm
    glad::glad
    OpenGL::GL
    imgui::imgui
)

# Automatically copy the resources folder to the build directory after every build
add_custom_command(
    TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Engine>/resources"
)

set_property(TARGET Engine PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )

endif()
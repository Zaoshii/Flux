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
    include/engine.hpp
    include/rendering/vertex_array.hpp
    include/rendering/index_buffer.hpp
    include/rendering/vertex_buffer.hpp
    include/rendering/Shader.hpp
    include/core/timer.hpp
    include/rendering/mesh.hpp
    include/rendering/renderer_types.hpp
    src/engine.cpp
    src/core/timer.cpp
    include/rendering/camera.hpp
    src/rendering/camera.cpp
    src/rendering/shader.cpp
    include/rendering/vertex.hpp
    include/rendering/cube.hpp
    src/rendering/cube.cpp
    include/rendering/transform.hpp
    src/rendering/mesh.cpp
    include/core/log.hpp
    include/core/window.hpp
    include/core/input_manager.hpp
    src/core/input_manager.cpp
    src/core/window.cpp
    include/rendering/camera_settings.hpp
    include/core/file/file.hpp
    src/core/file/file.cpp
    include/core/window_user_data.hpp
    include/rendering/renderer.hpp
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
        -Wuninitialized
        -Wmaybe-uninitialized
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wconversion
        -Wsign-conversion
    )

endif()
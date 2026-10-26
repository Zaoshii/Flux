cmake_minimum_required(VERSION 3.23)

project(SkeletonEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(SkeletonEngine
    src/main.cpp
)

target_link_libraries(SkeletonEngine PRIVATE
    glfw
    glm::glm
    glad::glad
    OpenGL::GL
)

# Automatically copy the resources folder to the build directory after every build
add_custom_command(
    TARGET SkeletonEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:SkeletonEngine>/resources"
)

set_property(TARGET SkeletonEngine PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "C:/Users/Z230/Desktop/Code/C++"
)
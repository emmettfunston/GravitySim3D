cmake_minimum_required(VERSION 3.10)
project(GravitySim3D LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# GLAD loader (generated) source
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/GLUtilities.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/PhysicsEngine.cpp
    src/Camera.cpp
    src/ConfigLoader.cpp
    src/InteractiveGUI.cpp
    extern/imgui/imgui.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad        # OpenGL loader
        glfw        # window + input
        OpenGL::GL  # system OpenGL
        glm::glm    # math library
)

# — (Optional) Turn on extra compiler warnings for Clang/GCC —
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
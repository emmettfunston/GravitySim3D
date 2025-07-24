cmake_minimum_required(VERSION 3.10)
project(GravitySim3D LANGUAGES C CXX)

# — Use C++17 without compiler extensions —
set(CMAKE_CXX_STANDARD        17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)

# — Find externally installed libraries —
find_package(glfw3   REQUIRED)   # Window/context & input
find_package(glm     REQUIRED)   # Math library (header‑only)
find_package(OpenGL  REQUIRED)   # macOS ships its own OpenGL framework

# — GLAD loader (your downloaded source) —
add_library(glad STATIC
    extern/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    extern/glad/include
)

# — Your executable and its sources —
add_executable(${PROJECT_NAME}
    src/test.cpp
    src/GLUtilities.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/PhysicsEngine.cpp
    src/Camera.cpp
)

# — Link everything together —
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad          # our loader for modern GL functions
        glfw          # window + context + input
        OpenGL::GL    # Apple’s OpenGL framework
        glm::glm      # math helpers (vec/mat)
)

# — (Optional) Turn on some warnings —
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

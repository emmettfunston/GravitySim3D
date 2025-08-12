/*
Run in Terminal:

cd~/Documents/Repositories/GravitySim3D
cd build
cmake ..      # regenerates with the updated CMakeLists
make          # or cmake --build .
./GravitySim3D
*/
#include "GLUtilities.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "PhysicsEngine.hpp"
#include "Camera.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main() {
    GLFWwindow* window = nullptr;
    if(!initWindow(800, 600, "GravitySim3D", window)) return -1;
    
    std::cout << "Window initialized successfully!" << std::endl;
    
    Shader shader("../shaders/basic.vs.glsl", "../shaders/basic.fs.glsl"); 
    Shader backgroundShader("../shaders/background.vs.glsl", "../shaders/background.fs.glsl");
    shader.use();
    
    // Create a simple sphere mesh (approximated with triangles)
    std::vector<float> sphereVertices;
    const int segments = 20; // More segments for smoother spheres
    const float radius = 0.15f; // Larger radius for better visibility
    
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            float theta1 = (float)i / segments * 2.0f * M_PI;
            float theta2 = (float)(i + 1) / segments * 2.0f * M_PI;
            float phi1 = (float)j / segments * M_PI;
            float phi2 = (float)(j + 1) / segments * M_PI;
            
            // Generate sphere vertices
            glm::vec3 v1(radius * sin(phi1) * cos(theta1), radius * cos(phi1), radius * sin(phi1) * sin(theta1));
            glm::vec3 v2(radius * sin(phi1) * cos(theta2), radius * cos(phi1), radius * sin(phi1) * sin(theta2));
            glm::vec3 v3(radius * sin(phi2) * cos(theta1), radius * cos(phi2), radius * sin(phi2) * sin(theta1));
            glm::vec3 v4(radius * sin(phi2) * cos(theta2), radius * cos(phi2), radius * sin(phi2) * sin(theta2));
            
            // Add triangles
            sphereVertices.insert(sphereVertices.end(), {v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z});
            sphereVertices.insert(sphereVertices.end(), {v2.x, v2.y, v2.z, v4.x, v4.y, v4.z, v3.x, v3.y, v3.z});
        }
    }
    
    Mesh sphere(sphereVertices);
    std::cout << "Sphere mesh created with " << sphereVertices.size() / 3 << " vertices" << std::endl;
    
    // Create background grid mesh
    std::vector<float> gridVertices;
    const float gridSize = 10.0f;
    const int gridSegments = 20;
    
    // Create a flat grid on the XZ plane
    for (int i = 0; i <= gridSegments; ++i) {
        float x = (float)i / gridSegments * gridSize - gridSize * 0.5f;
        // Vertical lines
        gridVertices.insert(gridVertices.end(), {x, -0.1f, -gridSize * 0.5f, x, -0.1f, gridSize * 0.5f});
        // Horizontal lines
        float z = (float)i / gridSegments * gridSize - gridSize * 0.5f;
        gridVertices.insert(gridVertices.end(), {-gridSize * 0.5f, -0.1f, z, gridSize * 0.5f, -0.1f, z});
    }
    
    Mesh grid(gridVertices, GL_LINES);
    std::cout << "Grid mesh created with " << gridVertices.size() / 3 << " vertices" << std::endl;

    PhysicsEngine phys;
    // Central body (more massive - like the sun)
    phys.addBody({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 20.0f });
    
    // Orbiting body with calculated orbital velocity for circular orbit
    float orbitalRadius = 6.0f;
    float orbitalVelocity = sqrt(0.02f * 20.0f / orbitalRadius) * 0.95f; // Adjusted for new gravity constant
    phys.addBody({ {orbitalRadius, 0.0f, 0.0f}, {0.0f, 0.0f, orbitalVelocity}, 1.0f });
    
    // Add a third body for more visual interest
    float outerRadius = 9.0f;
    float outerVelocity = sqrt(0.02f * 20.0f / outerRadius) * 0.9f; // Adjusted for new gravity constant
    phys.addBody({ {outerRadius, 0.0f, 0.0f}, {0.0f, 0.0f, outerVelocity}, 0.8f });
    
    std::cout << "Added 3 physics bodies" << std::endl;

    Camera cam({0, 8, 0}, -90, -90); // Top-down view for 2D simulation
    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    
    // Set cursor to center and capture it for mouse look
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "  Q/E - Move up/down" << std::endl;
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    // Set clear color to dark space
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    
    // Set the projection matrix uniform
    shader.setUniform("projection", proj);
    shader.setUniform("model", glm::mat4(1.0f));
    
    std::cout << "Starting render loop..." << std::endl;

    while(!glfwWindowShouldClose(window)) {
        float dt = 0.016f; // Fixed timestep for stability
        phys.update(dt);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw background grid first
        backgroundShader.use();
        backgroundShader.setUniform("projection", proj);
        backgroundShader.setUniform("view", cam.getViewMatrix());
        backgroundShader.setUniform("model", glm::mat4(1.0f));
        backgroundShader.setUniform("cameraPos", cam.getPosition());
        grid.draw();
        
        // Draw each physics body
        shader.use();
        shader.setUniform("projection", proj);
        shader.setUniform("view", cam.getViewMatrix());
        
        const auto& bodies = phys.getBodies();
        for (size_t i = 0; i < bodies.size(); ++i) {
            const auto& body = bodies[i];
            
            // Scale based on mass for visual effect
            float scale = 0.5f + body.mass * 0.3f;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), body.position);
            model = glm::scale(model, glm::vec3(scale));
            
            shader.setUniform("model", model);
            
            // Set different colors for different bodies
            if (i == 0) {
                shader.setUniform("bodyColor", glm::vec3(1.0f, 0.7f, 0.2f)); // Bright orange
            } else if (i == 1) {
                shader.setUniform("bodyColor", glm::vec3(0.2f, 0.8f, 1.0f)); // Blue
            } else {
                shader.setUniform("bodyColor", glm::vec3(0.8f, 0.2f, 0.9f)); // Purple
            }
            
            sphere.draw();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanupWindow(window);
    return 0;
}
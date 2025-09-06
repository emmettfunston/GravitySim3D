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
#include "ConfigLoader.hpp"
#include "InteractiveGUI.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main() {
    GLFWwindow* window = nullptr;
    if(!initWindow(800, 600, "GravitySim3D", window)) return -1;
    
    std::cout << "Window initialized successfully!" << std::endl;
    
    // Initialize Interactive GUI
    InteractiveGUI::init(window);
    
    // Load configuration
    SimulationConfig config = ConfigLoader::loadConfig("../config/simulation.json");
    bool configChanged = true; // Start with true to initialize physics engine
    
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
    
    // Function to recreate physics engine when config changes
    auto recreatePhysicsEngine = [&]() {
        phys = PhysicsEngine(); // Clear and recreate
        for (const auto& objConfig : config.objects) {
            // Calculate orbital velocity for orbiting objects
            glm::vec3 velocity = objConfig.velocity;
            if (objConfig.type == "orbiting") {
                float orbitalRadius = glm::length(objConfig.position);
                float orbitalVelocity = sqrt(config.physics.gravityConstant * config.objects[0].mass / orbitalRadius) * 0.9f;
                velocity = glm::vec3(0.0f, 0.0f, orbitalVelocity);
            }
            
            phys.addBody({objConfig.position, velocity, objConfig.mass});
            std::cout << "Added " << objConfig.name << " at position " 
                      << objConfig.position.x << ", " << objConfig.position.y << ", " << objConfig.position.z << std::endl;
        }
        std::cout << "Recreated physics engine with " << config.objects.size() << " bodies" << std::endl;
    };
    
    // Initial creation
    recreatePhysicsEngine();
    
    Camera cam({0, 6, 8}, -90, -45); // Angled view, not directly above
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
        
        // Recreate physics engine if config changed
        if (configChanged) {
            recreatePhysicsEngine();
            configChanged = false;
        }
        
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
        for (size_t i = 0; i < bodies.size() && i < config.objects.size(); ++i) {
            const auto& body = bodies[i];
            const auto& objConfig = config.objects[i];
            
            // Scale based on mass and configured radius for visual effect
            float scale = objConfig.radius * 2.0f; // Scale the configured radius
            glm::mat4 model = glm::translate(glm::mat4(1.0f), body.position);
            model = glm::scale(model, glm::vec3(scale));
            
            shader.setUniform("model", model);
            
            // Use configured color for each object
            shader.setUniform("bodyColor", objConfig.color);
            
            sphere.draw();
        }
        
        // Render Interactive GUI
        InteractiveGUI::render(config, configChanged);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    InteractiveGUI::shutdown();
    cleanupWindow(window);
    return 0;
}
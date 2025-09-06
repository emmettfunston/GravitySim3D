#include "ConfigLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

SimulationConfig ConfigLoader::loadConfig(const std::string& filename) {
    SimulationConfig config;
    
    // For now, we'll use hardcoded values that match the JSON
    // In a full implementation, you'd use a proper JSON library like nlohmann/json
    
    // Physics configuration
    config.physics.gravityConstant = 0.02f;
    config.physics.damping = 0.999f;
    config.physics.maxVelocity = 1.2f;
    config.physics.minDistance = 1.2f;
    config.physics.boundaryRadius = 12.0f;
    
    // Visual configuration
    config.visual.netGridSize = 15.0f;
    config.visual.deformationStrength = 0.8f;
    config.visual.shadowSize = 30.0f;
    config.visual.shadowOpacity = 0.6f;
    
    // Objects configuration
    ObjectConfig sun;
    sun.name = "sun";
    sun.type = "central";
    sun.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.mass = 20.0f;
    sun.radius = 0.2f;
    sun.color = glm::vec3(1.0f, 0.7f, 0.2f);
    sun.description = "Central massive object (like the sun)";
    config.objects.push_back(sun);
    
    ObjectConfig planet1;
    planet1.name = "planet1";
    planet1.type = "orbiting";
    planet1.position = glm::vec3(3.0f, 0.0f, 0.0f);
    planet1.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    planet1.mass = 1.0f;
    planet1.radius = 0.15f;
    planet1.color = glm::vec3(0.2f, 0.8f, 1.0f);
    planet1.description = "First orbiting planet (blue)";
    config.objects.push_back(planet1);
    
    ObjectConfig planet2;
    planet2.name = "planet2";
    planet2.type = "orbiting";
    planet2.position = glm::vec3(4.5f, 0.0f, 0.0f);
    planet2.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    planet2.mass = 0.8f;
    planet2.radius = 0.13f;
    planet2.color = glm::vec3(0.8f, 0.2f, 0.9f);
    planet2.description = "Second orbiting planet (purple)";
    config.objects.push_back(planet2);
    
    ObjectConfig planet3;
    planet3.name = "planet3";
    planet3.type = "orbiting";
    planet3.position = glm::vec3(6.0f, 0.0f, 0.0f);
    planet3.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    planet3.mass = 0.6f;
    planet3.radius = 0.12f;
    planet3.color = glm::vec3(0.9f, 0.4f, 0.8f);
    planet3.description = "Third orbiting planet (pink)";
    config.objects.push_back(planet3);
    
    ObjectConfig asteroid1;
    asteroid1.name = "asteroid1";
    asteroid1.type = "asteroid";
    asteroid1.position = glm::vec3(2.0f, 0.0f, 2.0f);
    asteroid1.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    asteroid1.mass = 0.1f;
    asteroid1.radius = 0.08f;
    asteroid1.color = glm::vec3(0.8f, 0.8f, 0.8f);
    asteroid1.description = "Small asteroid (gray)";
    config.objects.push_back(asteroid1);
    
    std::cout << "Loaded configuration with " << config.objects.size() << " objects" << std::endl;
    return config;
}

void ConfigLoader::saveConfig(const std::string& filename, const SimulationConfig& config) {
    // Implementation for saving config would go here
    std::cout << "Config save not implemented yet" << std::endl;
} 
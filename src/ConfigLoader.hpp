#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct ObjectConfig {
    std::string name;
    std::string type;
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float radius;
    glm::vec3 color;
    std::string description;
};

struct PhysicsConfig {
    float gravityConstant;
    float damping;
    float maxVelocity;
    float minDistance;
    float boundaryRadius;
};

struct VisualConfig {
    float netGridSize;
    float deformationStrength;
    float shadowSize;
    float shadowOpacity;
};

struct SimulationConfig {
    std::vector<ObjectConfig> objects;
    PhysicsConfig physics;
    VisualConfig visual;
};

class ConfigLoader {
public:
    static SimulationConfig loadConfig(const std::string& filename);
    static void saveConfig(const std::string& filename, const SimulationConfig& config);
    
private:
    static ObjectConfig parseObject(const std::string& jsonStr);
    static PhysicsConfig parsePhysics(const std::string& jsonStr);
    static VisualConfig parseVisual(const std::string& jsonStr);
}; 
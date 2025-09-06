#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "ConfigLoader.hpp"

class ImGuiWrapper {
public:
    static void init(GLFWwindow* window);
    static void beginFrame();
    static void renderControls(SimulationConfig& config, bool& configChanged);
    static void endFrame();
    static void shutdown();
    
private:
    static void renderObjectControls(std::vector<ObjectConfig>& objects, bool& configChanged);
    static void renderPhysicsControls(PhysicsConfig& physics, bool& configChanged);
    static void renderVisualControls(VisualConfig& visual, bool& configChanged);
    static void renderPresetButtons(SimulationConfig& config, bool& configChanged);
}; 
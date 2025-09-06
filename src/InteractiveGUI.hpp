#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "ConfigLoader.hpp"

struct GUIButton {
    float x, y, width, height;
    std::string label;
    bool pressed;
};

struct GUISlider {
    float x, y, width, height;
    std::string label;
    float* value;
    float minVal, maxVal;
    bool dragging;
};

struct GUIColorPicker {
    float x, y, width, height;
    std::string label;
    glm::vec3* color;
    bool active;
};

class InteractiveGUI {
public:
    static void init(GLFWwindow* window);
    static void render(SimulationConfig& config, bool& configChanged);
    static void handleMouse(double xpos, double ypos, bool leftPressed);
    static void shutdown();
    
private:
    static std::vector<GUIButton> buttons;
    static std::vector<GUISlider> sliders;
    static std::vector<GUIColorPicker> colorPickers;
    static float windowX, windowY, windowWidth, windowHeight;
    static double mouseX, mouseY;
    static bool mousePressed;
    static int currentTab;
    
    static void drawWindow();
    static void drawTabs();
    static void drawObjectsTab(SimulationConfig& config, bool& configChanged);
    static void drawPhysicsTab(SimulationConfig& config, bool& configChanged);
    static void drawVisualsTab(SimulationConfig& config, bool& configChanged);
    static void drawPresetsTab(SimulationConfig& config, bool& configChanged);
    
    static bool drawButton(float x, float y, float width, float height, const std::string& label);
    static bool drawSlider(float x, float y, float width, float height, const std::string& label, float* value, float minVal, float maxVal);
    static bool drawColorPicker(float x, float y, float width, float height, const std::string& label, glm::vec3* color);
    static void drawText(float x, float y, const std::string& text);
    static void drawRect(float x, float y, float width, float height, float r, float g, float b, float a);
}; 
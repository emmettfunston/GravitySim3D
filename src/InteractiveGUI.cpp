#include "InteractiveGUI.hpp"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <iostream>
#include <algorithm>

// Static member definitions
std::vector<GUIButton> InteractiveGUI::buttons;
std::vector<GUISlider> InteractiveGUI::sliders;
std::vector<GUIColorPicker> InteractiveGUI::colorPickers;
float InteractiveGUI::windowX = 600.0f;
float InteractiveGUI::windowY = 20.0f;
float InteractiveGUI::windowWidth = 190.0f;
float InteractiveGUI::windowHeight = 550.0f;
double InteractiveGUI::mouseX = 0.0;
double InteractiveGUI::mouseY = 0.0;
bool InteractiveGUI::mousePressed = false;
int InteractiveGUI::currentTab = 0;

void InteractiveGUI::init(GLFWwindow* window) {
    // Set up mouse callback
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            mousePressed = (action == GLFW_PRESS);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            handleMouse(xpos, ypos, mousePressed);
        }
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        handleMouse(xpos, ypos, mousePressed);
    });
}

void InteractiveGUI::render(SimulationConfig& config, bool& configChanged) {
    // Set up 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    drawWindow();
    drawTabs();
    
    // Draw tab content based on current tab
    switch (currentTab) {
        case 0: drawObjectsTab(config, configChanged); break;
        case 1: drawPhysicsTab(config, configChanged); break;
        case 2: drawVisualsTab(config, configChanged); break;
        case 3: drawPresetsTab(config, configChanged); break;
    }
    
    // Restore 3D rendering
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void InteractiveGUI::handleMouse(double xpos, double ypos, bool leftPressed) {
    mouseX = xpos;
    mouseY = ypos;
    mousePressed = leftPressed;
    
    // Check tab clicks
    if (leftPressed) {
        float tabWidth = windowWidth / 4.0f;
        if (ypos >= windowY + 5 && ypos <= windowY + 35) {
            for (int i = 0; i < 4; ++i) {
                if (xpos >= windowX + i * tabWidth && xpos <= windowX + (i + 1) * tabWidth) {
                    currentTab = i;
                    break;
                }
            }
        }
    }
}

void InteractiveGUI::shutdown() {
    buttons.clear();
    sliders.clear();
    colorPickers.clear();
}

void InteractiveGUI::drawWindow() {
    // Draw main window background
    drawRect(windowX, windowY, windowWidth, windowHeight, 0.1f, 0.1f, 0.15f, 0.95f);
    
    // Draw window border
    glColor4f(0.4f, 0.4f, 0.6f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(windowX, windowY);
    glVertex2f(windowX + windowWidth, windowY);
    glVertex2f(windowX + windowWidth, windowY + windowHeight);
    glVertex2f(windowX, windowY + windowHeight);
    glEnd();
}

void InteractiveGUI::drawTabs() {
    const char* tabNames[] = {"Objects", "Physics", "Visual", "Presets"};
    float tabWidth = windowWidth / 4.0f;
    
    for (int i = 0; i < 4; ++i) {
        float tabX = windowX + i * tabWidth;
        float tabY = windowY + 5;
        
        // Draw tab background
        if (i == currentTab) {
            drawRect(tabX, tabY, tabWidth, 30, 0.3f, 0.3f, 0.5f, 1.0f);
        } else {
            drawRect(tabX, tabY, tabWidth, 30, 0.2f, 0.2f, 0.3f, 1.0f);
        }
        
        // Draw tab border
        glColor4f(0.5f, 0.5f, 0.7f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(tabX, tabY);
        glVertex2f(tabX + tabWidth, tabY);
        glVertex2f(tabX + tabWidth, tabY + 30);
        glVertex2f(tabX, tabY + 30);
        glEnd();
        
        // Draw tab text (simplified)
        drawText(tabX + 5, tabY + 20, tabNames[i]);
    }
}

void InteractiveGUI::drawObjectsTab(SimulationConfig& config, bool& configChanged) {
    float yPos = windowY + 50;
    
    drawText(windowX + 10, yPos, "Objects: " + std::to_string(config.objects.size()));
    yPos += 25;
    
    // Add/Remove buttons
    if (drawButton(windowX + 10, yPos, 70, 25, "Add Object")) {
        ObjectConfig newObj;
        newObj.name = "Object" + std::to_string(config.objects.size() + 1);
        newObj.type = "orbiting";
        newObj.position = glm::vec3(5.0f, 0.0f, 0.0f);
        newObj.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        newObj.mass = 1.0f;
        newObj.radius = 0.15f;
        newObj.color = glm::vec3(0.5f, 0.5f, 0.5f);
        newObj.description = "New object";
        config.objects.push_back(newObj);
        configChanged = true;
    }
    
    if (drawButton(windowX + 100, yPos, 70, 25, "Remove")) {
        if (!config.objects.empty()) {
            config.objects.pop_back();
            configChanged = true;
        }
    }
    yPos += 35;
    
    // Object controls (show first 3 objects for space)
    for (size_t i = 0; i < std::min(config.objects.size(), size_t(3)); ++i) {
        auto& obj = config.objects[i];
        
        drawText(windowX + 10, yPos, obj.name);
        yPos += 20;
        
        // Mass slider
        if (drawSlider(windowX + 10, yPos, 160, 15, "Mass", &obj.mass, 0.1f, 50.0f)) {
            configChanged = true;
        }
        yPos += 25;
        
        // Radius slider
        if (drawSlider(windowX + 10, yPos, 160, 15, "Size", &obj.radius, 0.05f, 0.5f)) {
            configChanged = true;
        }
        yPos += 25;
        
        // Color picker
        if (drawColorPicker(windowX + 10, yPos, 160, 20, "Color", &obj.color)) {
            configChanged = true;
        }
        yPos += 30;
        
        // Position sliders
        if (drawSlider(windowX + 10, yPos, 50, 15, "X", &obj.position.x, -10.0f, 10.0f)) {
            configChanged = true;
        }
        if (drawSlider(windowX + 70, yPos, 50, 15, "Z", &obj.position.z, -10.0f, 10.0f)) {
            configChanged = true;
        }
        yPos += 35;
    }
}

void InteractiveGUI::drawPhysicsTab(SimulationConfig& config, bool& configChanged) {
    float yPos = windowY + 50;
    
    drawText(windowX + 10, yPos, "Physics Settings");
    yPos += 25;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Gravity", &config.physics.gravityConstant, 0.001f, 0.1f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Damping", &config.physics.damping, 0.9f, 1.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Max Velocity", &config.physics.maxVelocity, 0.1f, 5.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Min Distance", &config.physics.minDistance, 0.1f, 3.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Boundary", &config.physics.boundaryRadius, 5.0f, 20.0f)) {
        configChanged = true;
    }
}

void InteractiveGUI::drawVisualsTab(SimulationConfig& config, bool& configChanged) {
    float yPos = windowY + 50;
    
    drawText(windowX + 10, yPos, "Visual Settings");
    yPos += 25;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Grid Size", &config.visual.netGridSize, 5.0f, 50.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Deformation", &config.visual.deformationStrength, 0.1f, 2.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Shadow Size", &config.visual.shadowSize, 10.0f, 100.0f)) {
        configChanged = true;
    }
    yPos += 30;
    
    if (drawSlider(windowX + 10, yPos, 160, 15, "Shadow Opacity", &config.visual.shadowOpacity, 0.1f, 1.0f)) {
        configChanged = true;
    }
}

void InteractiveGUI::drawPresetsTab(SimulationConfig& config, bool& configChanged) {
    float yPos = windowY + 50;
    
    drawText(windowX + 10, yPos, "Preset Configurations");
    yPos += 25;
    
    if (drawButton(windowX + 10, yPos, 160, 30, "Solar System")) {
        config.objects.clear();
        
        // Sun
        ObjectConfig sun;
        sun.name = "Sun";
        sun.type = "central";
        sun.position = glm::vec3(0.0f, 0.0f, 0.0f);
        sun.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        sun.mass = 30.0f;
        sun.radius = 0.25f;
        sun.color = glm::vec3(1.0f, 0.8f, 0.0f);
        sun.description = "Central star";
        config.objects.push_back(sun);
        
        // Earth
        ObjectConfig earth;
        earth.name = "Earth";
        earth.type = "orbiting";
        earth.position = glm::vec3(4.0f, 0.0f, 0.0f);
        earth.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        earth.mass = 1.0f;
        earth.radius = 0.15f;
        earth.color = glm::vec3(0.2f, 0.6f, 1.0f);
        earth.description = "Blue planet";
        config.objects.push_back(earth);
        
        // Mars
        ObjectConfig mars;
        mars.name = "Mars";
        mars.type = "orbiting";
        mars.position = glm::vec3(6.0f, 0.0f, 0.0f);
        mars.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        mars.mass = 0.6f;
        mars.radius = 0.12f;
        mars.color = glm::vec3(0.8f, 0.3f, 0.2f);
        mars.description = "Red planet";
        config.objects.push_back(mars);
        
        configChanged = true;
    }
    yPos += 40;
    
    if (drawButton(windowX + 10, yPos, 160, 30, "Binary Stars")) {
        config.objects.clear();
        
        // Star 1
        ObjectConfig star1;
        star1.name = "Star 1";
        star1.type = "central";
        star1.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        star1.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        star1.mass = 25.0f;
        star1.radius = 0.2f;
        star1.color = glm::vec3(1.0f, 0.5f, 0.0f);
        star1.description = "Orange star";
        config.objects.push_back(star1);
        
        // Star 2
        ObjectConfig star2;
        star2.name = "Star 2";
        star2.type = "central";
        star2.position = glm::vec3(2.0f, 0.0f, 0.0f);
        star2.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        star2.mass = 20.0f;
        star2.radius = 0.18f;
        star2.color = glm::vec3(0.8f, 0.2f, 0.8f);
        star2.description = "Purple star";
        config.objects.push_back(star2);
        
        configChanged = true;
    }
    yPos += 40;
    
    if (drawButton(windowX + 10, yPos, 160, 30, "Asteroid Belt")) {
        config.objects.clear();
        
        // Central mass
        ObjectConfig central;
        central.name = "Central";
        central.type = "central";
        central.position = glm::vec3(0.0f, 0.0f, 0.0f);
        central.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        central.mass = 15.0f;
        central.radius = 0.2f;
        central.color = glm::vec3(1.0f, 0.7f, 0.2f);
        central.description = "Central mass";
        config.objects.push_back(central);
        
        // Add multiple asteroids
        for (int i = 0; i < 6; ++i) {
            ObjectConfig asteroid;
            asteroid.name = "Asteroid" + std::to_string(i + 1);
            asteroid.type = "asteroid";
            float angle = (float)i * 3.14159f * 2.0f / 6.0f;
            float radius = 3.0f + (float)(i % 3) * 1.0f;
            asteroid.position = glm::vec3(cos(angle) * radius, 0.0f, sin(angle) * radius);
            asteroid.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            asteroid.mass = 0.1f;
            asteroid.radius = 0.05f + (float)(i % 3) * 0.02f;
            asteroid.color = glm::vec3(0.6f, 0.6f, 0.6f);
            asteroid.description = "Small asteroid";
            config.objects.push_back(asteroid);
        }
        
        configChanged = true;
    }
}

bool InteractiveGUI::drawButton(float x, float y, float width, float height, const std::string& label) {
    bool isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
    bool wasClicked = isHovered && mousePressed;
    
    // Draw button background
    if (isHovered) {
        drawRect(x, y, width, height, 0.4f, 0.4f, 0.6f, 1.0f);
    } else {
        drawRect(x, y, width, height, 0.3f, 0.3f, 0.5f, 1.0f);
    }
    
    // Draw button border
    glColor4f(0.6f, 0.6f, 0.8f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw button text
    drawText(x + 5, y + height/2 + 5, label);
    
    return wasClicked;
}

bool InteractiveGUI::drawSlider(float x, float y, float width, float height, const std::string& label, float* value, float minVal, float maxVal) {
    bool changed = false;
    bool isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
    
    if (isHovered && mousePressed) {
        float newValue = minVal + (mouseX - x) / width * (maxVal - minVal);
        newValue = std::max(minVal, std::min(maxVal, newValue));
        if (*value != newValue) {
            *value = newValue;
            changed = true;
        }
    }
    
    // Draw slider track
    drawRect(x, y, width, height, 0.3f, 0.3f, 0.4f, 1.0f);
    
    // Draw slider handle
    float handlePos = x + (*value - minVal) / (maxVal - minVal) * width;
    handlePos = std::max(x, std::min(x + width, handlePos));
    
    drawRect(handlePos - 3, y - 2, 6, height + 4, 0.6f, 0.6f, 0.9f, 1.0f);
    
    // Draw label and value
    drawText(x, y - 15, label + ": " + std::to_string(*value).substr(0, 4));
    
    return changed;
}

bool InteractiveGUI::drawColorPicker(float x, float y, float width, float height, const std::string& label, glm::vec3* color) {
    bool changed = false;
    
    // Draw color preview
    drawRect(x, y, width, height, color->r, color->g, color->b, 1.0f);
    
    // Simple color cycling on click
    bool isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
    if (isHovered && mousePressed) {
        // Cycle through some preset colors
        static int colorIndex = 0;
        glm::vec3 colors[] = {
            glm::vec3(1.0f, 0.0f, 0.0f), // Red
            glm::vec3(0.0f, 1.0f, 0.0f), // Green
            glm::vec3(0.0f, 0.0f, 1.0f), // Blue
            glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
            glm::vec3(1.0f, 0.0f, 1.0f), // Magenta
            glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
            glm::vec3(1.0f, 0.5f, 0.0f), // Orange
            glm::vec3(0.5f, 0.0f, 1.0f)  // Purple
        };
        colorIndex = (colorIndex + 1) % 8;
        *color = colors[colorIndex];
        changed = true;
    }
    
    // Draw border
    glColor4f(0.5f, 0.5f, 0.7f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    drawText(x, y - 15, label);
    
    return changed;
}

void InteractiveGUI::drawText(float x, float y, const std::string& text) {
    // Simple text rendering - just a placeholder for now
    // In a full implementation, you'd use a proper text rendering system
}

void InteractiveGUI::drawRect(float x, float y, float width, float height, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
} 
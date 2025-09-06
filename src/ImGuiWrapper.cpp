#include "ImGuiWrapper.hpp"
#include "../extern/imgui/imgui.h"
#include <iostream>

void ImGuiWrapper::init(GLFWwindow* window) {
    ImGui::IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGui::ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw::InitForOpenGL(window, true);
    ImGui_ImplOpenGL3::Init("#version 330");
}

void ImGuiWrapper::beginFrame() {
    ImGui_ImplOpenGL3::NewFrame();
    ImGui_ImplGlfw::NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::renderControls(SimulationConfig& config, bool& configChanged) {
    ImGui::Begin("Simulation Controls", nullptr, ImGui::ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::BeginTabBar("ControlTabs")) {
        if (ImGui::BeginTabItem("Objects")) {
            renderObjectControls(config.objects, configChanged);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Physics")) {
            renderPhysicsControls(config.physics, configChanged);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Visual")) {
            renderVisualControls(config.visual, configChanged);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Presets")) {
            renderPresetButtons(config, configChanged);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void ImGuiWrapper::renderObjectControls(std::vector<ObjectConfig>& objects, bool& configChanged) {
    ImGui::Text("Objects: %zu", objects.size());
    
    if (ImGui::Button("Add Object")) {
        ObjectConfig newObj;
        newObj.name = "Object" + std::to_string(objects.size() + 1);
        newObj.type = "orbiting";
        newObj.position = glm::vec3(5.0f, 0.0f, 0.0f);
        newObj.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        newObj.mass = 1.0f;
        newObj.radius = 0.15f;
        newObj.color = glm::vec3(0.5f, 0.5f, 0.5f);
        newObj.description = "New object";
        objects.push_back(newObj);
        configChanged = true;
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Remove Last") && !objects.empty()) {
        objects.pop_back();
        configChanged = true;
    }
    
    ImGui::Separator();
    
    for (size_t i = 0; i < objects.size(); ++i) {
        auto& obj = objects[i];
        bool objectChanged = false;
        
        ImGui::PushID(&i);
        
        if (ImGui::CollapsingHeader(obj.name.c_str())) {
            // Name
            char nameBuf[128];
            strcpy(nameBuf, obj.name.c_str());
            if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
                obj.name = nameBuf;
                objectChanged = true;
            }
            
            // Type
            const char* types[] = {"central", "orbiting", "asteroid"};
            int currentType = 0;
            if (obj.type == "orbiting") currentType = 1;
            else if (obj.type == "asteroid") currentType = 2;
            
            if (ImGui::Combo("Type", &currentType, types, 3)) {
                obj.type = types[currentType];
                objectChanged = true;
            }
            
            // Position
            float pos[3] = {obj.position.x, obj.position.y, obj.position.z};
            if (ImGui::DragFloat3("Position", pos, 0.1f)) {
                obj.position = glm::vec3(pos[0], pos[1], pos[2]);
                objectChanged = true;
            }
            
            // Mass
            if (ImGui::DragFloat("Mass", &obj.mass, 0.1f, 0.1f, 100.0f)) {
                objectChanged = true;
            }
            
            // Radius
            if (ImGui::DragFloat("Radius", &obj.radius, 0.01f, 0.01f, 1.0f)) {
                objectChanged = true;
            }
            
            // Color
            float color[3] = {obj.color.r, obj.color.g, obj.color.b};
            if (ImGui::ColorEdit3("Color", color)) {
                obj.color = glm::vec3(color[0], color[1], color[2]);
                objectChanged = true;
            }
            
            // Delete button
            if (ImGui::Button("Delete Object")) {
                objects.erase(objects.begin() + i);
                configChanged = true;
                ImGui::PopID();
                break;
            }
        }
        
        ImGui::PopID();
        
        if (objectChanged) {
            configChanged = true;
        }
    }
}

void ImGuiWrapper::renderPhysicsControls(PhysicsConfig& physics, bool& configChanged) {
    ImGui::Text("Physics Settings");
    
    if (ImGui::DragFloat("Gravity Constant", &physics.gravityConstant, 0.001f, 0.001f, 1.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Damping", &physics.damping, 0.001f, 0.9f, 1.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Max Velocity", &physics.maxVelocity, 0.1f, 0.1f, 10.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Min Distance", &physics.minDistance, 0.1f, 0.1f, 5.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Boundary Radius", &physics.boundaryRadius, 0.5f, 5.0f, 20.0f)) {
        configChanged = true;
    }
}

void ImGuiWrapper::renderVisualControls(VisualConfig& visual, bool& configChanged) {
    ImGui::Text("Visual Settings");
    
    if (ImGui::DragFloat("Net Grid Size", &visual.netGridSize, 1.0f, 5.0f, 50.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Deformation Strength", &visual.deformationStrength, 0.1f, 0.1f, 2.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Shadow Size", &visual.shadowSize, 1.0f, 10.0f, 100.0f)) {
        configChanged = true;
    }
    
    if (ImGui::DragFloat("Shadow Opacity", &visual.shadowOpacity, 0.1f, 0.1f, 1.0f)) {
        configChanged = true;
    }
}

void ImGuiWrapper::renderPresetButtons(SimulationConfig& config, bool& configChanged) {
    ImGui::Text("Preset Configurations");
    
    if (ImGui::Button("Solar System")) {
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
    
    ImGui::SameLine();
    
    if (ImGui::Button("Binary Stars")) {
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
    
    ImGui::SameLine();
    
    if (ImGui::Button("Asteroid Belt")) {
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
        for (int i = 0; i < 8; ++i) {
            ObjectConfig asteroid;
            asteroid.name = "Asteroid" + std::to_string(i + 1);
            asteroid.type = "asteroid";
            float angle = (float)i * 3.14159f * 2.0f / 8.0f;
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

void ImGuiWrapper::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3::RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::shutdown() {
    ImGui_ImplOpenGL3::Shutdown();
    ImGui_ImplGlfw::Shutdown();
    ImGui::DestroyContext();
} 
#include "imgui.h"
#include <iostream>
#include <map>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace ImGui {
    // GUI state
    static bool s_initialized = false;
    static std::map<std::string, bool> s_windows;
    static std::map<std::string, bool> s_tabs;
    static std::string s_currentTab = "";
    static float s_windowX = 600.0f;
    static float s_windowY = 50.0f;
    static float s_currentY = 0.0f;
    static int s_idStack = 0;
    
    void IMGUI_CHECKVERSION() { s_initialized = true; }
    void CreateContext() {}
    void DestroyContext() {}
    void StyleColorsDark() {}
    
    ImGuiIO& GetIO() {
        static ImGuiIO io;
        return io;
    }
    
    void NewFrame() {
        s_currentY = s_windowY + 30.0f;
    }
    
    void Render() {
        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Set up 2D rendering
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1); // Screen coordinates
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glDisable(GL_DEPTH_TEST);
        
        // Restore 3D rendering
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        
        glDisable(GL_BLEND);
    }
    
    void* GetDrawData() { return nullptr; }
    
    void Begin(const char* name, bool* p_open, int flags) {
        s_windows[name] = true;
        
        // Draw window background
        glColor4f(0.1f, 0.1f, 0.15f, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX, s_windowY);
        glVertex2f(s_windowX + 180.0f, s_windowY);
        glVertex2f(s_windowX + 180.0f, s_windowY + 400.0f);
        glVertex2f(s_windowX, s_windowY + 400.0f);
        glEnd();
        
        // Draw window border
        glColor4f(0.4f, 0.4f, 0.6f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(s_windowX, s_windowY);
        glVertex2f(s_windowX + 180.0f, s_windowY);
        glVertex2f(s_windowX + 180.0f, s_windowY + 400.0f);
        glVertex2f(s_windowX, s_windowY + 400.0f);
        glEnd();
        
        s_currentY = s_windowY + 30.0f;
    }
    
    void End() {
        // Window is complete
    }
    
    void Text(const char* fmt, ...) {
        // Simple text rendering placeholder
        s_currentY += 20.0f;
    }
    
    void Separator() {
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 170.0f, s_currentY);
        glEnd();
        s_currentY += 10.0f;
    }
    
    bool Button(const char* label) {
        float buttonWidth = 80.0f;
        float buttonHeight = 25.0f;
        
        // Draw button background
        glColor4f(0.3f, 0.3f, 0.5f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 10.0f + buttonWidth, s_currentY);
        glVertex2f(s_windowX + 10.0f + buttonWidth, s_currentY + buttonHeight);
        glVertex2f(s_windowX + 10.0f, s_currentY + buttonHeight);
        glEnd();
        
        // Draw button border
        glColor4f(0.6f, 0.6f, 0.8f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 10.0f + buttonWidth, s_currentY);
        glVertex2f(s_windowX + 10.0f + buttonWidth, s_currentY + buttonHeight);
        glVertex2f(s_windowX + 10.0f, s_currentY + buttonHeight);
        glEnd();
        
        s_currentY += buttonHeight + 5.0f;
        return false; // Simplified - no actual click detection
    }
    
    void SameLine(float offset_from_start_x, float spacing) {
        s_currentY -= 30.0f; // Move back up for same line
    }
    
    bool InputText(const char* label, char* buf, size_t buf_size, int flags) {
        Text(label);
        
        // Draw text input box
        glColor4f(0.2f, 0.2f, 0.3f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY + 20.0f);
        glVertex2f(s_windowX + 10.0f, s_currentY + 20.0f);
        glEnd();
        
        s_currentY += 25.0f;
        return false;
    }
    
    bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, int flags) {
        Text(label);
        
        // Draw slider track
        glColor4f(0.3f, 0.3f, 0.4f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY + 15.0f);
        glVertex2f(s_windowX + 10.0f, s_currentY + 15.0f);
        glEnd();
        
        // Draw slider handle (simplified position)
        float handlePos = s_windowX + 10.0f + (*v / v_max) * 150.0f;
        if (handlePos > s_windowX + 160.0f) handlePos = s_windowX + 160.0f;
        
        glColor4f(0.6f, 0.6f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(handlePos - 3.0f, s_currentY - 2.0f);
        glVertex2f(handlePos + 3.0f, s_currentY - 2.0f);
        glVertex2f(handlePos + 3.0f, s_currentY + 17.0f);
        glVertex2f(handlePos - 3.0f, s_currentY + 17.0f);
        glEnd();
        
        s_currentY += 25.0f;
        return false;
    }
    
    bool DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, int flags) {
        Text(label);
        
        // Draw three small sliders for X, Y, Z
        for (int i = 0; i < 3; ++i) {
            glColor4f(0.3f, 0.3f, 0.4f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2f(s_windowX + 10.0f + i * 50.0f, s_currentY);
            glVertex2f(s_windowX + 55.0f + i * 50.0f, s_currentY);
            glVertex2f(s_windowX + 55.0f + i * 50.0f, s_currentY + 15.0f);
            glVertex2f(s_windowX + 10.0f + i * 50.0f, s_currentY + 15.0f);
            glEnd();
            
            // Handle
            float handlePos = s_windowX + 10.0f + i * 50.0f + (v[i] / v_max) * 45.0f;
            glColor4f(0.6f, 0.6f, 0.9f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2f(handlePos - 2.0f, s_currentY - 1.0f);
            glVertex2f(handlePos + 2.0f, s_currentY - 1.0f);
            glVertex2f(handlePos + 2.0f, s_currentY + 16.0f);
            glVertex2f(handlePos - 2.0f, s_currentY + 16.0f);
            glEnd();
        }
        
        s_currentY += 25.0f;
        return false;
    }
    
    bool ColorEdit3(const char* label, float col[3], int flags) {
        Text(label);
        
        // Draw color preview
        glColor4f(col[0], col[1], col[2], 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY + 20.0f);
        glVertex2f(s_windowX + 10.0f, s_currentY + 20.0f);
        glEnd();
        
        s_currentY += 25.0f;
        return false;
    }
    
    bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items) {
        Text(label);
        
        // Draw combo box
        glColor4f(0.2f, 0.2f, 0.3f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 10.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY);
        glVertex2f(s_windowX + 160.0f, s_currentY + 20.0f);
        glVertex2f(s_windowX + 10.0f, s_currentY + 20.0f);
        glEnd();
        
        s_currentY += 25.0f;
        return false;
    }
    
    bool CollapsingHeader(const char* label, int flags) {
        // Draw collapsible header
        glColor4f(0.4f, 0.4f, 0.6f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 5.0f, s_currentY);
        glVertex2f(s_windowX + 175.0f, s_currentY);
        glVertex2f(s_windowX + 175.0f, s_currentY + 25.0f);
        glVertex2f(s_windowX + 5.0f, s_currentY + 25.0f);
        glEnd();
        
        s_currentY += 30.0f;
        return true; // Always expanded for simplicity
    }
    
    bool BeginTabBar(const char* str_id, int flags) {
        // Draw tab bar background
        glColor4f(0.2f, 0.2f, 0.3f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(s_windowX + 5.0f, s_currentY);
        glVertex2f(s_windowX + 175.0f, s_currentY);
        glVertex2f(s_windowX + 175.0f, s_currentY + 30.0f);
        glVertex2f(s_windowX + 5.0f, s_currentY + 30.0f);
        glEnd();
        
        s_currentY += 35.0f;
        return true;
    }
    
    void EndTabBar() {
        // Tab bar complete
    }
    
    bool BeginTabItem(const char* label, bool* p_open, int flags) {
        s_tabs[label] = true;
        s_currentTab = label;
        return true; // Always show tab content for simplicity
    }
    
    void EndTabItem() {
        s_currentY += 10.0f; // Add some spacing
    }
    
    void PushID(const void* ptr_id) { s_idStack++; }
    void PopID() { s_idStack--; }
}

namespace ImGui_ImplGlfw {
    void InitForOpenGL(void* window, bool install_callbacks) {}
    void Shutdown() {}
    void NewFrame() {}
}

namespace ImGui_ImplOpenGL3 {
    void Init(const char* glsl_version) {}
    void Shutdown() {}
    void NewFrame() {}
    void RenderDrawData(void* draw_data) {}
} 
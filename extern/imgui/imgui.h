#pragma once
#include <string>
#include <vector>
#include <functional>

// Simplified ImGui implementation
namespace ImGui {
    struct ImVec2 { float x, y; ImVec2(float _x = 0, float _y = 0) : x(_x), y(_y) {} };
    struct ImVec4 { float x, y, z, w; ImVec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z), w(_w) {} };
    
    // Constants
    static const int ImGuiWindowFlags_AlwaysAutoResize = 1;
    static const int ImGuiConfigFlags_NavEnableKeyboard = 1;
    
    void Begin(const char* name, bool* p_open = nullptr, int flags = 0);
    void End();
    void Text(const char* fmt, ...);
    void Separator();
    bool Button(const char* label);
    void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
    bool InputText(const char* label, char* buf, size_t buf_size, int flags = 0);
    bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0);
    bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0);
    bool ColorEdit3(const char* label, float col[3], int flags = 0);
    bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool CollapsingHeader(const char* label, int flags = 0);
    bool BeginTabBar(const char* str_id, int flags = 0);
    void EndTabBar();
    bool BeginTabItem(const char* label, bool* p_open = nullptr, int flags = 0);
    void EndTabItem();
    void PushID(const void* ptr_id);
    void PopID();
    
    // Styling
    void StyleColorsDark();
    
    // IO
    struct ImGuiIO {
        int ConfigFlags;
        ImGuiIO() : ConfigFlags(0) {}
    };
    ImGuiIO& GetIO();
    
    // Context
    void CreateContext();
    void DestroyContext();
    void NewFrame();
    void Render();
    void* GetDrawData();
    
    // Check version
    void IMGUI_CHECKVERSION();
}

// Implementation placeholders
namespace ImGui_ImplGlfw {
    void InitForOpenGL(void* window, bool install_callbacks);
    void Shutdown();
    void NewFrame();
}

namespace ImGui_ImplOpenGL3 {
    void Init(const char* glsl_version);
    void Shutdown();
    void NewFrame();
    void RenderDrawData(void* draw_data);
} 
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Initializes GLFW, glad, and creates a window
bool initWindow(int width, int height, const char* title, GLFWwindow*& window);
// Cleans up and terminates GLFW and destroys the window
void cleanupWindow(GLFWwindow* window);
#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 pos, float yaw, float pitch);
    glm::mat4 getViewMatrix() const;
    void processKeyboard(int key, float dt);
    void processMouse(float xoff, float yoff);
    static void keyCallback(GLFWwindow*, int, int, int, int);
    static void mouseCallback(GLFWwindow*, double, double);

private:
    glm::vec3 position, front, up;
    float yaw, pitch, speed, sensitivity;
    static Camera* instance;

};
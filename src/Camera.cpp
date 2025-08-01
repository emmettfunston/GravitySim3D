#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera*Camera::instance = nullptr;
Camera::Camera(glm::vec3 pos, float y, float p) : position(pos), yaw(y), pitch(p), speed(10.0f), sensitivity(0.1f) {
    front = {0, 0, -1};
    up = {0, 1, 0};
    instance = this;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(int key, float dt) {
    float v = speed * dt;
    if (key == GLFW_KEY_W) position += front * v;
    if (key == GLFW_KEY_S) position -= front * v;
    if (key == GLFW_KEY_A) position -= glm::normalize(glm::cross(front, up)) * v;
    if (key == GLFW_KEY_D) position += glm::normalize(glm::cross(front, up)) * v;
}

void Camera::processMouse(float xoff, float yoff) {
    xoff *= sensitivity;
    yoff *= sensitivity; 
    yaw += xoff;
    pitch += yoff;
    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;
    glm::vec3 d;
    d.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    d.y = sin(glm::radians(pitch));
    d.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(d);
}

void Camera::keyCallback(GLFWwindow*, int key, int, int action, int) {
    if (instance&&(action == GLFW_PRESS || action == GLFW_REPEAT)) instance->processKeyboard(key, float(glfwGetTime()));
}

void Camera::mouseCallback(GLFWwindow*, double xpos, double ypos) {
    static double lx=400, ly=300;
    double xoff = xpos - lx, yoff = ly - ypos;
    lx = xpos;
    ly = ypos;
    if(instance) instance->processMouse((float)xoff, (float)yoff);
}
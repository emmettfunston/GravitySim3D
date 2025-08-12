#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera*Camera::instance = nullptr;
Camera::Camera(glm::vec3 pos, float y, float p) : position(pos), yaw(y), pitch(p), speed(1.0f), sensitivity(0.05f) {
    // Calculate front vector based on yaw and pitch
    glm::vec3 d;
    d.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    d.y = sin(glm::radians(pitch));
    d.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(d);
    up = {0, 1, 0};
    instance = this;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::processKeyboard(int key, float dt) {
    float v = speed * dt;
    if (key == GLFW_KEY_W) position += front * v;
    if (key == GLFW_KEY_S) position -= front * v;
    if (key == GLFW_KEY_A) position -= glm::normalize(glm::cross(front, up)) * v;
    if (key == GLFW_KEY_D) position += glm::normalize(glm::cross(front, up)) * v;
    if (key == GLFW_KEY_Q) position += up * v; // Move up
    if (key == GLFW_KEY_E) position -= up * v; // Move down
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

void Camera::keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (instance&&(action == GLFW_PRESS || action == GLFW_REPEAT)) instance->processKeyboard(key, float(glfwGetTime()));
}

void Camera::mouseCallback(GLFWwindow*, double xpos, double ypos) {
    static double lx = 0, ly = 0;
    static bool firstMouse = true;
    
    if (firstMouse) {
        lx = xpos;
        ly = ypos;
        firstMouse = false;
    }
    
    double xoff = xpos - lx, yoff = ly - ypos;
    lx = xpos;
    ly = ypos;
    if(instance) instance->processMouse((float)xoff, (float)yoff);
}
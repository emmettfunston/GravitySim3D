/*
Run in Terminal:

cd~/Documents/Repositories/GravitySim3D
cd build
cmake ..      # regenerates with the updated CMakeLists
make          # or cmake --build .
./GravitySim3D
*/

int main() {
    GLFWwindow* window = nullptr;
    if(!initWindow(800, 600, "GravitySim3D", window)) return -1;
    
    Shader shader("shaders/basic.vs.glsl", "shaders/basic.fs.glsl"); 
    shaders.use();
    
    std::vector<float> tri = {{-0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f}};
    Mesh triangle(tri);

    PhysicsEngine phys; 
    phys.addBody({0,0,0}, {0,0,0}, 1.0f);
    phys.addBody({1,0,0}, {0,0,0}, 1.0f);

    Camera cam({0,0,3}, -90, 0);
    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    shader.setUniform("projection", proj);

    while(!glfwWindowShouldClose(window)) {
        float dt = (float)glfwGetTime();
        phys.update(dt);
        shader.use();
        shader.setUniform("view", cam.getViewMatrix());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanupWindow(window);
    return 0;
}
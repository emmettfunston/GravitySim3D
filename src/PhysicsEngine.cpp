#include "PhysicsEngine.hpp"
#include <cmath>
#include <glm/glm.hpp>

void PhysicsEngine::addBody(const Body& b) {
    bodies.push_back(b);
}
void PhysicsEngine::update(float dt) {
    for (size_t i = 0; i < bodies.size(); ++i) {
        glm::vec3 accel(0.0f);
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
            glm::vec3 dir=bodies[j].position - bodies[i].position;
            float dist2 = glm::dot(dir, dir) + 1e-6f;
            float invDist = 1.0f / sqrt(dist2);
            float force = G * bodies[i].mass * bodies[j].mass * invDist * invDist;
            accel += dir * (force/bodies[i].mass*invDist);
            }
        }
        bodies[i].velocity += accel * dt;
    }
    for (auto& b : bodies) b.position += b.velocity*dt;
}
const std::vector<PhysicsEngine::Body>& PhysicsEngine::getBodies() const {
    return bodies;
}
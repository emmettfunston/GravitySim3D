#include "PhysicsEngine.hpp"
#include <cmath>
#include <glm/glm.hpp>

void PhysicsEngine::addBody(const Body& b) {
    bodies.push_back(b);
}

// Calculate orbital velocity for circular orbit
float calculateOrbitalVelocity(float centralMass, float radius, float gravityConstant) {
    return sqrt(gravityConstant * centralMass / radius);
}

void PhysicsEngine::update(float dt) {
    // Calculate orbital velocities for bodies that should be orbiting
    if (bodies.size() >= 2) {
        float centralMass = bodies[0].mass;
        float gravityConstant = 0.02f; // Adjusted for stable orbits
        
        // Update orbital velocities for orbiting bodies
        for (size_t i = 1; i < bodies.size(); ++i) {
            float currentRadius = glm::length(bodies[i].position - bodies[0].position);
            float targetVelocity = calculateOrbitalVelocity(centralMass, currentRadius, gravityConstant);
            
            // Get current velocity direction
            glm::vec3 currentVel = bodies[i].velocity;
            float currentSpeed = glm::length(currentVel);
            
            // Calculate tangential direction (perpendicular to radial direction)
            glm::vec3 radialDir = glm::normalize(bodies[i].position - bodies[0].position);
            glm::vec3 tangentialDir = glm::vec3(-radialDir.z, 0.0f, radialDir.x); // Perpendicular in XZ plane
            
            // Adjust velocity to maintain orbital motion
            if (currentSpeed > 0.1f) {
                // Gradually adjust toward orbital velocity
                glm::vec3 targetVel = tangentialDir * targetVelocity;
                bodies[i].velocity = glm::mix(currentVel, targetVel, 0.01f);
            } else {
                // If nearly stationary, set initial orbital velocity
                bodies[i].velocity = tangentialDir * targetVelocity;
            }
        }
    }
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        glm::vec3 accel(0.0f);
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
            glm::vec3 dir=bodies[j].position - bodies[i].position;
            float dist2 = glm::dot(dir, dir) + 1e-6f;
            float invDist = 1.0f / sqrt(dist2);
            // Use the same gravity constant as orbital calculations
            float force = 0.02f * bodies[i].mass * bodies[j].mass * invDist * invDist;
            accel += glm::normalize(dir) * (force/bodies[i].mass);
            }
        }
        
        // Constrain movement to XZ plane (Y = 0)
        accel.y = 0.0f;
        bodies[i].velocity.y = 0.0f;
        
        bodies[i].velocity += accel * dt;
        
        // Add some damping to prevent chaotic behavior
        bodies[i].velocity *= 0.999f;
        
        // Keep objects within bounds (prevent them from flying off screen)
        const float maxDistance = 12.0f;
        if (glm::length(bodies[i].position) > maxDistance) {
            glm::vec3 direction = glm::normalize(bodies[i].position);
            bodies[i].position = direction * maxDistance;
            // Reverse velocity component away from center
            float radialVel = glm::dot(bodies[i].velocity, direction);
            if (radialVel > 0) {
                bodies[i].velocity -= direction * radialVel * 0.5f;
            }
        }
        
        // Limit maximum velocity to prevent chaos
        float maxVel = 1.2f;
        if (glm::length(bodies[i].velocity) > maxVel) {
            bodies[i].velocity = glm::normalize(bodies[i].velocity) * maxVel;
        }
    }
    
    // Handle collisions between bodies - extremely aggressive separation
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            glm::vec3 diff = bodies[i].position - bodies[j].position;
            float dist = glm::length(diff);
            float minDist = 1.2f; // Much larger minimum distance between bodies
            
            if (dist < minDist) {
                // Separate bodies very aggressively to prevent any overlap
                glm::vec3 separation = glm::normalize(diff) * (minDist - dist);
                bodies[i].position += separation * 1.8f;
                bodies[j].position -= separation * 1.8f;
                
                // Bounce velocities very strongly to prevent sticking
                glm::vec3 normal = glm::normalize(diff);
                float velDiff = glm::dot(bodies[i].velocity - bodies[j].velocity, normal);
                if (velDiff < 0) {
                    bodies[i].velocity -= normal * velDiff * 1.0f;
                    bodies[j].velocity += normal * velDiff * 1.0f;
                }
                
                // Add significant random velocity to break out of stuck states
                if (dist < minDist * 0.7f) {
                    bodies[i].velocity += glm::vec3(0.4f, 0.0f, 0.4f) * (float)(rand() % 100) / 100.0f;
                    bodies[j].velocity += glm::vec3(0.4f, 0.0f, 0.4f) * (float)(rand() % 100) / 100.0f;
                }
            }
        }
    }
    
    for (auto& b : bodies) {
        b.position += b.velocity*dt;
        // Ensure all bodies stay on the XZ plane
        b.position.y = 0.0f;
    }
}
const std::vector<PhysicsEngine::Body>& PhysicsEngine::getBodies() const {
    return bodies;
}
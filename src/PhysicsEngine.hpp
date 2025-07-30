#pragma once
#include <glm/glm.hpp>
#include <vector>

class PhysicsEngine {
    public: 
        struct Body {
            glm::vec3 position;
            glm::vec3 velocity;
            float mass;
        };
        void addBody(const Body& b);
        void update(float dt);
        const std::vector<Body>& getBodies() const;
    
    private:
        std::vector<Body> bodies;
        static constexpr double G = 6.67430e-11;
};
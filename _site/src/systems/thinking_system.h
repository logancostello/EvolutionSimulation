#pragma once
#include <entt/entt.hpp>

class ThinkingSystem {
    public:
        ThinkingSystem(entt::registry& registry);
        void update(float dt);
        
    private:
        entt::registry& registry;
};

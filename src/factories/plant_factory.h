#pragma once
#include <entt/entt.hpp>

class PlantFactory {
    public:
        PlantFactory(entt::registry& registry);
        void spawn_random();
        
    private:
        entt::registry& registry;
};
#pragma once
#include <entt/entt.hpp>

class PlantFactory {
    public:
        PlantFactory(entt::registry& registry);
        void spawn_random(int world_size_x, int world_size_y);
        
    private:
        entt::registry& registry;
};
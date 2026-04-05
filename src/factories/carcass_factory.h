#pragma once
#include <entt/entt.hpp>

class CarcassFactory {
    public:
        CarcassFactory(entt::registry& registry);
        void spawn_carcass(entt::entity creature);
        
    private:
        entt::registry& registry;
};
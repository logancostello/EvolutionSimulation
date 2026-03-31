#pragma once
#include <entt/entt.hpp>
#include "factories/brain_factory.h"

class CreatureFactory {
    public:
        CreatureFactory(entt::registry& registry);
        void spawn_random(int world_size_x, int world_size_y, float time);
        entt::entity spawn_child(entt::entity parent, float time);
        
    private:
        entt::registry& registry;
        BrainFactory brain_factory;
};
#pragma once
#include <entt/entt.hpp>
#include "factories/brain_factory.h"

class CreatureFactory {
    public:
        CreatureFactory(entt::registry& registry);
        void spawn_random();
        entt::entity spawn_child(entt::entity parent);
        
    private:
        entt::registry& registry;
        BrainFactory brain_factory;
};
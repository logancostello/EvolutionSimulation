#pragma once
#include <entt/entt.hpp>
#include "factories/creature_factory.h"

class ReproductionSystem {
    public:
        ReproductionSystem(entt::registry& registry, CreatureFactory& creature_factory);
        void update(float time);
    private:
        entt::registry& registry;
        CreatureFactory& creature_factory;
};

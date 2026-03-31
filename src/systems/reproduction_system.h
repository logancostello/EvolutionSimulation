#pragma once
#include <entt/entt.hpp>
#include "factories/creature_factory.h"
#include "mutators/brain_mutator.h"

class ReproductionSystem {
    public:
        ReproductionSystem(entt::registry& registry, CreatureFactory& creature_factory, BrainMutator& brain_mutator);
        void update(float time);
    private:
        entt::registry& registry;
        CreatureFactory& creature_factory;
        BrainMutator& brain_mutator;
};

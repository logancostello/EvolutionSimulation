#pragma once
#include <entt/entt.hpp>
#include "factories/creature_factory.h"
#include "mutators/brain_mutator.h"
#include "mutators/genome_mutator.h"

class ReproductionSystem {
    public:
        ReproductionSystem(
            entt::registry& registry,
            CreatureFactory& creature_factory,
            BrainMutator& brain_mutator,
            GenomeMutator& genome_mutator
        );
        void update();
    private:
        entt::registry& registry;
        CreatureFactory& creature_factory;
        BrainMutator& brain_mutator;
        GenomeMutator& genome_mutator;
};

#include "reproduction_system.h"
#include "components/components.h"

const float ENERGY_FOR_REPRODUCTION = 20.0f;

ReproductionSystem::ReproductionSystem(entt::registry& registry, CreatureFactory& creature_factory, BrainMutator& brain_mutator) 
    : registry(registry) 
    , creature_factory(creature_factory)
    , brain_mutator(brain_mutator)
{};

void ReproductionSystem::update() {
    auto view = registry.view<ChildEnergy>();

    std::vector<entt::entity> to_reproduce;
    for (auto [entity, child_energy] : view.each()) {
        if (child_energy.energy >= ENERGY_FOR_REPRODUCTION) {
            child_energy.energy -= ENERGY_FOR_REPRODUCTION;
            to_reproduce.push_back(entity);
        }
    }

    for (auto entity : to_reproduce) {
        entt::entity child = creature_factory.spawn_child(entity);
        brain_mutator.mutate(child);
    }
}

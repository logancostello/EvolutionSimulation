#include "systems/digestion_system.h"
#include "components/components.h"

const float DIGESTION_RATE = 1.5f;

DigestionSystem::DigestionSystem(entt::registry& registry) : registry(registry) {};

void DigestionSystem::update(float dt) {
    auto view = registry.view<Stomach, CreatureEnergy, ChildEnergy>();
    for (auto [entity, stomach, energy, child_energy] : view.each()) {
        float needed_energy = energy.max - energy.energy;
        float digested_energy = std::min(stomach.potential_energy, DIGESTION_RATE * dt);

        float energy_for_pool = std::min(needed_energy, digested_energy);
        float extra_energy = digested_energy - energy_for_pool;

        stomach.potential_energy -= digested_energy;
        energy.energy += energy_for_pool;
        child_energy.energy += extra_energy;
    }
}
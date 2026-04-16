#include "systems/digestion_system.h"
#include "components/components.h"

const float DIGESTION_RATE = 1.5f;

DigestionSystem::DigestionSystem(entt::registry& registry) : registry(registry) {};

void DigestionSystem::update(float dt) {
    auto view = registry.view<Stomach, CreatureEnergy>();
    for (auto [entity, stomach, energy] : view.each()) {
        float needed_energy = energy.max - energy.energy;
        float max_digestable = std::min(stomach.potential_energy, DIGESTION_RATE * dt);
        float actually_digested = std::min(needed_energy, max_digestable);
        stomach.potential_energy -= actually_digested;
        energy.energy += actually_digested;
    }
}
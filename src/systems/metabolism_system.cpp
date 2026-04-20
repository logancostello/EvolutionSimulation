#include "systems/metabolism_system.h"
#include "components/components.h"

MetabolismSystem::MetabolismSystem(entt::registry& registry) : registry(registry) {};

void MetabolismSystem::update(float dt) {

    auto view = registry.view<CreatureEnergy, Velocity>();
    for (auto [entity, energy, vel] : view.each()) {

        // Basic living cost
        energy.energy -= 0.5 * dt;

        // Speed Cost
        energy.energy -= 0.75 * vel.mag * vel.mag * dt;
        
        // Mark creature dead if no energy
        if (energy.energy <= 0) {
            registry.emplace<Dead>(entity);
        }
    }
}
#include "systems/metabolism_system.h"
#include "components.h"

MetabolismSystem::MetabolismSystem(entt::registry& registry) : registry(registry) {};

void MetabolismSystem::update(float dt) {

    // Remove energy each frame (for now fixed amount)
    auto view = registry.view<Energy>();
    for (auto [entity, energy] : view.each()) {
        energy.energy -= 1 * dt;

        // Mark creature dead if no energy
        if (energy.energy <= 0) {
            registry.emplace<Dead>(entity);
        }
    }
}
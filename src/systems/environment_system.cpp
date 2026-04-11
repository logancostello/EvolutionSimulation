#include "systems/environment_system.h"
#include "components/components.h"

const float CARCASS_DECAY_RATE = 0.01;

EnvironmentSystem::EnvironmentSystem(entt::registry& registry) : registry(registry) {};

void EnvironmentSystem::update(float dt) {
    auto view = registry.view<FoodEnergy, Size, Carcass>();
    for (auto [entity, food_energy, size] : view.each()) {
        food_energy.energy /= (1.0f + CARCASS_DECAY_RATE * dt);
        size.radius /= (1.0f + CARCASS_DECAY_RATE * dt);

        if (food_energy.energy < 0.01f) registry.emplace_or_replace<Dead>(entity);
    }
}
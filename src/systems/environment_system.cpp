#include "systems/environment_system.h"
#include "components/components.h"

const float CARCASS_DECAY_RATE = 0.01;
const float IMMIGRATION_TIME = 20.0f;
const int MIN_CREATURES = 25;

EnvironmentSystem::EnvironmentSystem(entt::registry& registry, CreatureFactory& creature_factory) 
    : registry(registry)
    , creature_factory(creature_factory) 
{};

void EnvironmentSystem::update(float dt, float x, float y, float time) {

    // Decay all carcasses
    auto view = registry.view<FoodEnergy, Size, Carcass>();
    for (auto [entity, food_energy, size] : view.each()) {
        food_energy.energy /= (1.0f + CARCASS_DECAY_RATE * dt);
        size.radius /= (1.0f + CARCASS_DECAY_RATE * dt);

        if (food_energy.energy < 0.01f) registry.emplace_or_replace<Dead>(entity);
    }

    // Check Immigration
    time_since_immigration += dt;
    if (time_since_immigration > IMMIGRATION_TIME) {
        time_since_immigration -= IMMIGRATION_TIME;
        creature_factory.spawn_random(x, y, time);
    }

    // Ensure always some creatures
    auto creature_view = registry.view<Creature>();
    int num_creatures = creature_view.size();
    while (num_creatures < MIN_CREATURES) {
        num_creatures++;
        creature_factory.spawn_random(x, y, time);
    }
}
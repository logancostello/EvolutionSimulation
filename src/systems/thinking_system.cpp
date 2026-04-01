#include "systems/thinking_system.h"
#include "components/components.h"

ThinkingSystem::ThinkingSystem(entt::registry& registry) : registry(registry) {};

void ThinkingSystem::update(float dt) {
    auto view = registry.view<Brain>();

    for (auto [entity, brain] : view.each()) {
        brain.load_inputs(registry, entity);
        brain.think(dt);
        brain.populate_outputs(registry, entity);
    }
}
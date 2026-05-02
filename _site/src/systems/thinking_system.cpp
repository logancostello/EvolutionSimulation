#include "systems/thinking_system.h"
#include "components/components.h"

ThinkingSystem::ThinkingSystem(entt::registry& registry) : registry(registry) {};

void ThinkingSystem::update(float dt) {
    auto view = registry.view<Brain>();

    for (auto [entity, brain] : view.each()) {
        brain.think(dt, registry, entity);
        brain.populate_outputs(registry, entity);
    }
}
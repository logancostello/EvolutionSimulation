#include "systems/thinking_system.h"
#include "components/components.h"

ThinkingSystem::ThinkingSystem(entt::registry& registry) : registry(registry) {};

void ThinkingSystem::update() {
    auto view = registry.view<Brain>();

    for (auto [entity, brain] : view.each()) {
        brain.load_inputs(registry, entity);
        brain.think();
        brain.populate_outputs(registry, entity);
    }
}
#include "systems/cleanup_system.h"
#include <entt/entt.hpp>
#include "components.h"

CleanupSystem::CleanupSystem(entt::registry& registry) : registry(registry) {};

void CleanupSystem::update() {
    auto view = registry.view<Dead>();
    std::vector<entt::entity> to_destroy(view.begin(), view.end());

    for (auto entity : to_destroy) {
        registry.destroy(entity);
    }
}
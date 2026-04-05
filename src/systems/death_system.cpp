#include "systems/death_system.h"
#include <entt/entt.hpp>
#include "components/components.h"

DeathSystem::DeathSystem(entt::registry& registry) : registry(registry) {};

void DeathSystem::update() {
    auto view = registry.view<Dead>();
    std::vector<entt::entity> to_destroy(view.begin(), view.end());

    for (auto entity : to_destroy) {
        registry.destroy(entity);
    }
}
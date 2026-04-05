#include "systems/death_system.h"
#include <entt/entt.hpp>
#include "components/components.h"

DeathSystem::DeathSystem(entt::registry& registry, CarcassFactory& carcass_factory) : registry(registry), carcass_factory(carcass_factory) {};

void DeathSystem::update() {
    auto view = registry.view<Dead>();
    std::vector<entt::entity> to_destroy(view.begin(), view.end());

    for (auto entity : to_destroy) {

        if (registry.all_of<Creature>(entity)) {
            carcass_factory.spawn_carcass(entity);
        }

        registry.destroy(entity);
    }
}
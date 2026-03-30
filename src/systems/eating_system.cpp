#include "eating_system.h"
#include "components.h"
#include <cmath>

EatingSystem::EatingSystem(entt::registry& registry) : registry(registry) {};

void EatingSystem::update() {
    auto plant_view = registry.view<Position, Size, Plant>();
    auto creature_view = registry.view<Position, Size, Energy, Creature>();

    // Naive implementation for now
    for (auto [p_entity, p_pos, p_size] : plant_view.each()) {
        for (auto [c_entity, c_pos, c_size, c_energy] : creature_view.each()) {
            float sq_dist = std::pow(p_pos.x - c_pos.x, 2) + std::pow(p_pos.y - c_pos.y, 2);

            // Handle eating
            if (sq_dist < std::pow(p_size.radius + c_size.radius, 2)) {
                c_energy.energy += 1;
                registry.emplace<Dead>(p_entity);
                break;
            }
        }
    }
}
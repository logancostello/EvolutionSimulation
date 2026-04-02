#include "eating_system.h"
#include "components/components.h"
#include <cmath>

EatingSystem::EatingSystem(entt::registry& registry) : registry(registry) {};

void EatingSystem::update(QuadTree& lookup_tree) {

    auto view = registry.view<Position, Size, Energy, Creature>();
    for (auto [creature, c_pos, c_size, c_energy] : view.each()) {
        auto nearby = std::vector<entt::entity>{};
        lookup_tree.query(c_pos.x, c_pos.y, 2 * c_size.radius, nearby);

        for (entt::entity e : nearby) {
            if (!registry.all_of<Plant>(e)) continue;

            if (registry.all_of<Dead>(e)) continue;
            
            auto& p_pos = registry.get<Position>(e);
            auto& p_size = registry.get<Size>(e);
            
            float dx = c_pos.x - p_pos.x;
            float dy = c_pos.y - p_pos.y;
            float sqr_dist = dx * dx + dy * dy;
            float collision_dist = c_size.radius + p_size.radius;
            float sqr_collision_dist = collision_dist * collision_dist;
            
            if (sqr_dist < sqr_collision_dist) {
                c_energy.energy += 5;
                registry.emplace<Dead>(e);
                break;
            }
        }


    }

    // auto plant_view = registry.view<Position, Size, Plant>();
    // auto creature_view = registry.view<Position, Size, Energy, Creature>();

    // // Naive implementation for now
    // for (auto [p_entity, p_pos, p_size] : plant_view.each()) {
    //     for (auto [c_entity, c_pos, c_size, c_energy] : creature_view.each()) {
    //         float sq_dist = std::pow(p_pos.x - c_pos.x, 2) + std::pow(p_pos.y - c_pos.y, 2);

    //         // Handle eating
    //         if (sq_dist < std::pow(p_size.radius + c_size.radius, 2)) {
    //             c_energy.energy += 5;
    //             registry.emplace<Dead>(p_entity);
    //             break;
    //         }
    //     }
    // }
}
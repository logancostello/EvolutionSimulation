#include "collision_system.h"
#include "components/components.h"
#include <cmath>

CollisionSystem::CollisionSystem(entt::registry& registry) : registry(registry) {};

void CollisionSystem::update(QuadTree& lookup_tree) {


    auto movables = registry.view<Position, Size>();
    for (auto [a, a_pos, a_size] : movables.each()) {
        if (registry.all_of<Dead>(a)) continue;

        auto nearby = std::vector<entt::entity>{};
        lookup_tree.query(a_pos.x, a_pos.y, 2 * a_size.radius, nearby);

        for (entt::entity b : nearby) {
            if (b <= a) continue; // skip self and already-processed pairs
            if (registry.all_of<Dead>(b)) continue;

            auto& b_pos = registry.get<Position>(b);
            auto& b_size = registry.get<Size>(b);

            if (registry.all_of<Creature>(a) && registry.all_of<Plant>(b)) {
                handle_eating(a, b);
                if (registry.all_of<Dead>(b)) continue;
            }
            if (registry.all_of<Creature>(b) && registry.all_of<Plant>(a)) {
                handle_eating(b, a);
                if (registry.all_of<Dead>(a)) continue;
            }

            float dx = a_pos.x - b_pos.x;
            float dy = a_pos.y - b_pos.y;
            float sqr_dist = dx * dx + dy * dy;
            float collision_dist = a_size.radius + b_size.radius;

            if (sqr_dist >= collision_dist * collision_dist) continue;
            if (sqr_dist == 0.f) {
                a_pos.x += 0.1f;
                b_pos.x -= 0.1f;
                dx = a_pos.x - b_pos.x;
                dy = a_pos.y - b_pos.y;
                sqr_dist = dx * dx + dy * dy;
            }

            float dist = std::sqrt(sqr_dist);
            float overlap = collision_dist - dist;
            float nx = dx / dist;
            float ny = dy / dist;

            a_pos.x += nx * overlap * 0.5f;
            a_pos.y += ny * overlap * 0.5f;
            b_pos.x -= nx * overlap * 0.5f;
            b_pos.y -= ny * overlap * 0.5f;
        }
    }
}

void CollisionSystem::handle_eating(entt::entity creature, entt::entity plant) {

    if (registry.all_of<Dead>(creature) || registry.all_of<Dead>(plant)) return;

    auto& c_pos = registry.get<Position>(creature);
    auto& c_size = registry.get<Size>(creature);
    auto& c_energy = registry.get<Energy>(creature);

    auto& p_pos = registry.get<Position>(plant);
    auto& p_size = registry.get<Size>(plant);
    
    float dx = c_pos.x - p_pos.x;
    float dy = c_pos.y - p_pos.y;
    float sqr_dist = dx * dx + dy * dy;
    float collision_dist = c_size.radius + p_size.radius;
    float sqr_collision_dist = collision_dist * collision_dist;
    
    if (sqr_dist < sqr_collision_dist) {
        c_energy.energy += 5;
        registry.emplace<Dead>(plant);
    }
}
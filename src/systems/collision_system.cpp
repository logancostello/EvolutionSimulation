#include "collision_system.h"
#include "components/components.h"
#include <cmath>

CollisionSystem::CollisionSystem(entt::registry& registry) : registry(registry) {};

void CollisionSystem::update(QuadTree& lookup_tree, float dt) {


    auto movables = registry.view<Position, Size>();
    for (auto [a, a_pos, a_size] : movables.each()) {
        if (registry.all_of<Dead>(a)) continue;

        nearby.clear();
        lookup_tree.query(a_pos.x, a_pos.y, 2 * a_size.radius, nearby);

        for (entt::entity b : nearby) {
            if (b <= a) continue; // skip self and already-processed pairs
            if (registry.all_of<Dead>(b)) continue;

            auto& b_pos = registry.get<Position>(b);
            auto& b_size = registry.get<Size>(b);

            if (registry.all_of<Creature>(a) && registry.all_of<FoodEnergy>(b)) {
                handle_eating(a, b, dt);
                if (registry.all_of<Dead>(b)) continue;
            }
            if (registry.all_of<Creature>(b) && registry.all_of<FoodEnergy>(a)) {
                handle_eating(b, a, dt);
                if (registry.all_of<Dead>(a)) continue;
            }

            float dx = a_pos.x - b_pos.x;
            float dy = a_pos.y - b_pos.y;
            float sqr_dist = dx * dx + dy * dy;
            float collision_dist = a_size.radius + b_size.radius;

            if (sqr_dist >= collision_dist * collision_dist) continue;

            registry.emplace_or_replace<OldPosition>(a, a_pos.x, a_pos.y);
            registry.emplace_or_replace<OldPosition>(b, b_pos.x, b_pos.y);

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

void CollisionSystem::handle_eating(entt::entity creature, entt::entity food, float dt) {

    if (registry.all_of<Dead>(creature) || registry.all_of<Dead>(food)) return;

    auto& c_pos = registry.get<Position>(creature);
    auto& c_size = registry.get<Size>(creature);
    auto& c_stomach = registry.get<Stomach>(creature);
    auto& c_bite = registry.get<Bite>(creature);

    auto& f_pos = registry.get<Position>(food);
    auto& f_size = registry.get<Size>(food);
    auto& f_energy = registry.get<FoodEnergy>(food);
    
    float dx = c_pos.x - f_pos.x;
    float dy = c_pos.y - f_pos.y;
    float sqr_dist = dx * dx + dy * dy;
    float collision_dist = c_size.radius + f_size.radius;
    float sqr_collision_dist = collision_dist * collision_dist;
    
    if (sqr_dist < sqr_collision_dist) {

        float possible_consumable_energy = std::min(f_energy.energy, c_bite.energy_per_sec * dt);
        possible_consumable_energy = std::min(possible_consumable_energy, c_stomach.max - c_stomach.potential_energy);

        c_stomach.potential_energy += possible_consumable_energy;
        f_energy.energy -= possible_consumable_energy;

        if (f_energy.energy <= 0) registry.emplace<Dead>(food);
    }
}
#include "collision_system.h"
#include "components/components.h"
#include <cmath>

CollisionSystem::CollisionSystem(entt::registry& registry) : registry(registry) {};

void CollisionSystem::update(QuadTree& lookup_tree, float dt) {

    auto movables = registry.view<Position, Size>(entt::exclude<Dead>);
    for (auto [a, a_pos, a_size] : movables.each()) {

        touching.clear();
        lookup_tree.query(a_pos.x, a_pos.y, a_size.radius, touching);

        for (entt::entity b : touching) {
            if (b <= a) continue; // skip self and already-processed pairs
            if (registry.all_of<Dead>(b)) continue;

            auto& b_pos = registry.get<Position>(b);
            auto& b_size = registry.get<Size>(b);

            const bool a_is_creature = registry.all_of<Creature>(a);
            const bool b_is_creature = registry.all_of<Creature>(b);
            const bool a_is_food = registry.all_of<FoodEnergy>(a);
            const bool b_is_food = registry.all_of<FoodEnergy>(b);

            if (a_is_creature && b_is_food) {
                handle_eating(a, b, dt);
                if (registry.all_of<Dead>(b)) continue;
            }
            if (b_is_creature && a_is_food) {
                handle_eating(b, a, dt);
                if (registry.all_of<Dead>(a)) continue;
            }

            float dx = a_pos.x - b_pos.x;
            float dy = a_pos.y - b_pos.y;
            float sqr_dist = dx * dx + dy * dy;
            float collision_dist = a_size.radius + b_size.radius;

            if (sqr_dist == 0.f) {
                a_pos.x += 0.1f;
                b_pos.x -= 0.1f;
                dx = 0.2f;
                dy = 0.f;
                sqr_dist = 0.04f;
            }

            registry.emplace_or_replace<OldPosition>(a, a_pos.x, a_pos.y);
            registry.emplace_or_replace<OldPosition>(b, b_pos.x, b_pos.y);

            float dist = std::sqrt(sqr_dist);
            float inv_dist = 1.f / dist;
            float overlap = collision_dist - dist;
            float nx = dx * inv_dist;
            float ny = dy * inv_dist;
            float half_overlap = overlap * 0.5f;

            a_pos.x += nx * half_overlap;
            a_pos.y += ny * half_overlap;
            b_pos.x -= nx * half_overlap;
            b_pos.y -= ny * half_overlap;
        }
    }
}

void CollisionSystem::handle_eating(entt::entity creature, entt::entity food, float dt) {
    if (registry.all_of<Dead>(creature) || registry.all_of<Dead>(food)) return;

    auto& c_stomach = registry.get<Stomach>(creature);
    auto& c_bite = registry.get<Bite>(creature);
    auto& f_energy = registry.get<FoodEnergy>(food);

    float room = c_stomach.max - c_stomach.potential_energy;
    if (room <= 0.f) return;

    float bite = c_bite.energy_per_sec * dt;
    float consumed = std::min({f_energy.energy, bite, room});

    c_stomach.potential_energy += consumed;
    f_energy.energy -= consumed;

    if (f_energy.energy <= 0.f) registry.emplace<Dead>(food);
}
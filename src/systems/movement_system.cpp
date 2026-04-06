#include "systems/movement_system.h"
#include <entt/entt.hpp>
#include "components/components.h"

MovementSystem::MovementSystem(entt::registry& registry) : registry(registry) {};

void MovementSystem::update(float dt) {
    auto view = registry.view<Position, Velocity>();
    for (auto [entity, pos, vel] : view.each()) {
        vel.dir += vel.turn_rate * dt;
        pos.x += std::cos(vel.dir) * vel.mag * dt;
        pos.y += std::sin(vel.dir) * vel.mag * dt;
        registry.emplace_or_replace<DirtyPosition>(entity);
    }
}
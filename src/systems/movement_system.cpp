#include "systems/movement_system.h"
#include <entt/entt.hpp>
#include "components/components.h"

const float MAX_VEL = 100;
const float MAX_TURN_RATE = 3.14;

MovementSystem::MovementSystem(entt::registry& registry) : registry(registry) {};

void MovementSystem::update(float dt) {
    auto view = registry.view<Position, Velocity>();
    for (auto [entity, pos, vel] : view.each()) {
        registry.emplace_or_replace<OldPosition>(entity, pos.x, pos.y);
        vel.dir += vel.turn_rate * MAX_TURN_RATE * dt;
        pos.x += std::cos(vel.dir) * vel.mag * MAX_VEL * dt;
        pos.y += std::sin(vel.dir) * vel.mag * MAX_VEL * dt;
    }
}
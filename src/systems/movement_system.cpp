#include "systems/movement_system.h"
#include <entt/entt.hpp>
#include "components/components.h"

MovementSystem::MovementSystem(entt::registry& registry) : registry(registry) {};

void MovementSystem::update(float dt) {
    auto view = registry.view<Position, Velocity>();
    for (auto [entity, pos, vel] : view.each()) {
        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;
    }
}
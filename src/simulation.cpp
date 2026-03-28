#include "simulation.h"
#include "components.h"

void Simulation::update(entt::registry& registry, float dt) {

    auto view = registry.view<Position, Velocity>();

    for (auto [entity, pos, vel] : view.each()) {
        
        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;

    }
}
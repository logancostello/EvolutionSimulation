#include "simulation.h"
#include "components.h"

const int NUM_INIT_CREATURES = 10;
const int NUM_INIT_PLANTS = 100;

Simulation::Simulation(entt::registry& registry) 
    : registry(registry)
    , creature_factory(registry)
    , plant_factory(registry)
{}

void Simulation::initialize() {
    for (int i = 0; i < NUM_INIT_CREATURES; i++) {
        creature_factory.spawn_random();
    }

    for (int i = 0; i < NUM_INIT_PLANTS; i++) {
        plant_factory.spawn_random();
    }
}

void Simulation::update(float dt) {

    auto view = registry.view<Position, Velocity>();

    for (auto [entity, pos, vel] : view.each()) {
        
        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;

    }
}
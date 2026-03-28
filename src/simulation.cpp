#include "simulation.h"
#include "components.h"

const int NUM_INIT_CREATURES = 10;
const int NUM_INIT_PLANTS = 100;

Simulation::Simulation(entt::registry& registry) 
    : creature_factory(registry)
    , plant_factory(registry)
    , movement_system(registry)
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
    movement_system.update(dt);
}
#include "simulation.h"
#include "components.h"

const int WORLD_SIZE_X = 800;
const int WORLD_SIZE_Y = 600;
const int NUM_INIT_CREATURES = 10;
const int NUM_INIT_PLANTS = 100;

Simulation::Simulation(entt::registry& registry) 
    : world_size_x(WORLD_SIZE_X)
    , world_size_y(WORLD_SIZE_Y)
    , creature_factory(registry)
    , plant_factory(registry)
    , movement_system(registry)
{}

void Simulation::initialize() {
    for (int i = 0; i < NUM_INIT_CREATURES; i++) {
        creature_factory.spawn_random(this->world_size_x, this->world_size_y);
    }

    for (int i = 0; i < NUM_INIT_PLANTS; i++) {
        plant_factory.spawn_random(this->world_size_x, this->world_size_y);
    }
}

void Simulation::update(float dt) {
    movement_system.update(dt);
}
#include "simulation.h"
#include "components.h"

const int WORLD_SIZE_X = 2400;
const int WORLD_SIZE_Y = 1800;
const int NUM_INIT_CREATURES = 100;
const int NUM_INIT_PLANTS = 500;

Simulation::Simulation(entt::registry& registry) 
    : time(0.0f)
    , world_size_x(WORLD_SIZE_X)
    , world_size_y(WORLD_SIZE_Y)
    , creature_factory(registry)
    , plant_factory(registry)
    , movement_system(registry)
    , metabolism_system(registry)
    , cleanup_system(registry)
    , eating_system(registry)
    , reproduction_system(registry, creature_factory)
{}

void Simulation::initialize() {
    for (int i = 0; i < NUM_INIT_CREATURES; i++) {
        creature_factory.spawn_random(this->world_size_x, this->world_size_y, time);
    }

    for (int i = 0; i < NUM_INIT_PLANTS; i++) {
        plant_factory.spawn_random(this->world_size_x, this->world_size_y);
    }
}

void Simulation::update(float dt) {
    movement_system.update(dt);
    eating_system.update();
    metabolism_system.update(dt);
    reproduction_system.update(time);
    cleanup_system.update();

    time += dt;
}
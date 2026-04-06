#include "simulation.h"
#include "components/components.h"
#include "limits"

const int WORLD_SIZE_X = 2400;
const int WORLD_SIZE_Y = 1800;
const int NUM_INIT_CREATURES = 100;
const int NUM_INIT_PLANTS = 500;

Simulation::Simulation(entt::registry& registry) 
    : time(0.0f)
    , world_size_x(WORLD_SIZE_X)
    , world_size_y(WORLD_SIZE_Y)
    , entity_lookup_tree(world_size_x, world_size_y)
    , spatial_index_system(registry, entity_lookup_tree)
    , creature_factory(registry)
    , plant_factory(registry)
    , carcass_factory(registry)
    , brain_mutator(registry)
    , movement_system(registry)
    , metabolism_system(registry)
    , death_system(registry, carcass_factory)
    , collision_system(registry)
    , reproduction_system(registry, creature_factory, brain_mutator)
    , thinking_system(registry)
    , sensor_system(registry)
    , plant_system(plant_factory)
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

    spatial_index_system.update();
    plant_system.update(dt, world_size_x, world_size_y);
    sensor_system.update(entity_lookup_tree);
    thinking_system.update(dt);
    movement_system.update(dt);
    collision_system.update(entity_lookup_tree);
    metabolism_system.update(dt);
    reproduction_system.update(time);
    death_system.update();

    time += dt;
}
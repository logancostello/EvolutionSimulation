#include "simulation.h"
#include "components/components.h"
#include "limits"

const int WORLD_SIZE_X = 7500;
const int WORLD_SIZE_Y = 7500;
const int NUM_INIT_CREATURES = 500;
const int NUM_INIT_PLANTS = 2000;

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
    , plant_system(plant_factory, registry)
    , environment_system(registry, creature_factory)
    , digestion_system(registry)
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
    environment_system.update(dt, world_size_x, world_size_y, time);
    plant_system.update(dt, world_size_x, world_size_y);
    sensor_system.update(entity_lookup_tree);
    thinking_system.update(dt);
    movement_system.update(dt);
    collision_system.update(entity_lookup_tree, dt);
    digestion_system.update(dt);
    metabolism_system.update(dt);
    reproduction_system.update(time);
    death_system.update();

    time += dt;
}
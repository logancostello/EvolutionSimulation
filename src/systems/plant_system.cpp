#include "systems/plant_system.h"

const float TIME_PER_SPAWN = 0.15;

void PlantSystem::update(float dt, int world_size_x, int world_size_y) {
    accumulated_time += dt;

    while (accumulated_time > TIME_PER_SPAWN) {
        accumulated_time -= TIME_PER_SPAWN;
        plant_factory.spawn_random(world_size_x, world_size_y);
    }
}
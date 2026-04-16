#include "systems/plant_system.h"
#include "components/components.h"

const float TIME_PER_SPAWN = 0.15;
const int MAX_PLANTS = 2000;

void PlantSystem::update(float dt, int world_size_x, int world_size_y) {

    auto plants = registry.view<Plant>();
    if (plants.size() > MAX_PLANTS) return;

    accumulated_time += dt;

    while (accumulated_time > TIME_PER_SPAWN) {
        accumulated_time -= TIME_PER_SPAWN;
        plant_factory.spawn_random(world_size_x, world_size_y);
    }
}
#include "factories/plant_factory.h"
#include "components/components.h"
#include "random.h"

PlantFactory::PlantFactory(entt::registry& registry) : registry(registry) {};

void PlantFactory::spawn_random(int world_size_x, int world_size_y) {

    entt::entity plant = registry.create();
    registry.emplace<Position>(
        plant, 
        Random::float_range(world_size_x / -2.0f, world_size_x / 2.0f),
        Random::float_range(world_size_y / -2.0f, world_size_y / 2.0f)
    );
    registry.emplace<OldPosition>(plant);
    registry.emplace<Color>(plant, 66, 247, 98);
    registry.emplace<Size>(plant, 5.0f);
    registry.emplace<FoodEnergy>(plant, 5);
    registry.emplace<Plant>(plant);
}
#include "factories/plant_factory.h"
#include "components/components.h"
#include "random.h"
#include "fertility.h"

PlantFactory::PlantFactory(entt::registry& registry) : registry(registry) {};

void PlantFactory::spawn_random(int world_size_x, int world_size_y) {

    entt::entity plant = registry.create();
    auto [x, y] = Fertility::random_location();
    registry.emplace<Position>(plant, x, y);
    registry.emplace<OldPosition>(plant);
    registry.emplace<Color>(plant, 66, 247, 98);
    registry.emplace<Size>(plant, 5.0f);
    registry.emplace<FoodEnergy>(plant, 10);
    registry.emplace<Plant>(plant);
}
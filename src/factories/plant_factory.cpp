#include "factories/plant_factory.h"
#include "components.h"
#include <random>

PlantFactory::PlantFactory(entt::registry& registry) : registry(registry) {};

void PlantFactory::spawn_random() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_x_dist(0.0f, 800.0f);
    std::uniform_real_distribution<float> pos_y_dist(0.0f, 600.f);

    entt::entity plant = registry.create();
    registry.emplace<Position>(plant, pos_x_dist(gen), pos_y_dist(gen));
    registry.emplace<Color>(plant, 66, 247, 98);
    registry.emplace<Size>(plant, 2.0f);
}
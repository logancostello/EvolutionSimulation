#include "factories/creature_factory.h"
#include "components.h"
#include <random>

CreatureFactory::CreatureFactory(entt::registry& registry) : registry(registry) {};

void CreatureFactory::spawn_random(int world_size_x, int world_size_y) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_x_dist(world_size_x / -2.0f, world_size_x / 2.0f);
    std::uniform_real_distribution<float> pos_y_dist(world_size_y / -2.0f, world_size_y / 2.0f);
    std::uniform_real_distribution<float> vel_dist(-50.0f, 50.0f);

    entt::entity creature = registry.create();
    registry.emplace<Position>(creature, pos_x_dist(gen), pos_y_dist(gen));
    registry.emplace<Velocity>(creature, vel_dist(gen), vel_dist(gen));
    registry.emplace<Color>(creature, 255, 255, 255);
    registry.emplace<Size>(creature, 5.0f);
}
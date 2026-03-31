#include "factories/creature_factory.h"
#include "components/components.h"
#include <random>
#include <cmath>

CreatureFactory::CreatureFactory(entt::registry& registry) 
    : registry(registry)
    , brain_factory(registry)
{};

void CreatureFactory::spawn_random(int world_size_x, int world_size_y, float time) {

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
    registry.emplace<Energy>(creature, 10);
    registry.emplace<Creature>(creature);
    registry.emplace<TimeOf>(creature, time, time);
    
    brain_factory.create_basic_brain(creature);
}

void CreatureFactory::spawn_child(entt::entity parent, float time) {
    entt::entity child = registry.create();

    Position& parent_pos = registry.get<Position>(parent);
    Velocity& parent_vel = registry.get<Velocity>(parent);

    // Give child a position behind the parent
    float vel_mag = std::sqrt(std::pow(parent_vel.dx, 2) + std::pow(parent_vel.dy, 2));
    if (vel_mag < 0.001f) vel_mag = 1.0f;

    float norm_x = parent_vel.dx / vel_mag;
    float norm_y = parent_vel.dy / vel_mag;

    registry.emplace<Position>(child, parent_pos.x - norm_x * 20, parent_pos.y - norm_y * 20);

    registry.emplace<Velocity>(child, parent_vel.dx, parent_vel.dy);
    registry.emplace<Color>(child, 255, 255, 255);
    registry.emplace<Size>(child, 5.0f);
    registry.emplace<Energy>(child, 10);
    registry.emplace<Creature>(child);
    registry.emplace<TimeOf>(child, time, time); 
    
    brain_factory.create_basic_brain(child);
}
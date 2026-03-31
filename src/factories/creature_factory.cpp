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
    registry.emplace<VisionSensors>(creature, 0, 0);
    
    brain_factory.create_basic_brain(creature);
}


void CreatureFactory::spawn_child(entt::entity parent, float time) {
    entt::entity child = registry.create();

    Position& parent_pos = registry.get<Position>(parent);
    Velocity& parent_vel = registry.get<Velocity>(parent);

    // Give child a position behind the parent using direction
    float behind_angle = parent_vel.dir + M_PI;  // opposite of travel direction
    float spawn_x = parent_pos.x + std::cos(behind_angle) * 20.0f;
    float spawn_y = parent_pos.y + std::sin(behind_angle) * 20.0f;

    registry.emplace<Position>(child, spawn_x, spawn_y);
    registry.emplace<Velocity>(child, parent_vel.mag, parent_vel.dir);
    registry.emplace<Color>(child, 255, 255, 255);
    registry.emplace<Size>(child, 5.0f);
    registry.emplace<Energy>(child, 10.0f);
    registry.emplace<Creature>(child);
    registry.emplace<TimeOf>(child, time, time); 
    registry.emplace<VisionSensors>(child, 0.0f, 0.0f);

    brain_factory.create_basic_brain(child);
}
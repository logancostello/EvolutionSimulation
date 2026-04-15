#include "factories/creature_factory.h"
#include "components/components.h"
#include "random.h"
#include <cmath>

CreatureFactory::CreatureFactory(entt::registry& registry) 
    : registry(registry)
    , brain_factory(registry)
{};

void CreatureFactory::spawn_random(int world_size_x, int world_size_y, float time) {

    entt::entity creature = registry.create();
    registry.emplace<Position>(
        creature, 
        Random::float_range(world_size_x / -2.0f, world_size_x / 2.0f),
        Random::float_range(world_size_y / -2.0f, world_size_y / 2.0f)
    );
    registry.emplace<OldPosition>(creature);
    registry.emplace<Velocity>(creature, 0, 0);
    registry.emplace<Color>(creature, 255, 255, 255);
    registry.emplace<Size>(creature, 8.0f);
    registry.emplace<CreatureEnergy>(creature, 10, 20.0f);
    registry.emplace<Creature>(creature);
    registry.emplace<TimeOf>(creature, time, time);
    registry.emplace<VisionSensors>(creature, 0, 0);
    registry.emplace<Bite>(creature, 5);
    
    brain_factory.create_basic_brain(creature);
}


entt::entity CreatureFactory::spawn_child(entt::entity parent, float time) {
    entt::entity child = registry.create();

    Position& parent_pos = registry.get<Position>(parent);
    Velocity& parent_vel = registry.get<Velocity>(parent);
    Brain& parent_brain = registry.get<Brain>(parent);

    // Give child a position behind the parent using direction
    float behind_angle = parent_vel.dir + M_PI;  // opposite of travel direction
    float spawn_x = parent_pos.x + std::cos(behind_angle) * 20.0f;
    float spawn_y = parent_pos.y + std::sin(behind_angle) * 20.0f;

    registry.emplace<Position>(child, spawn_x, spawn_y);
    registry.emplace<OldPosition>(child);
    registry.emplace<Velocity>(child, parent_vel.mag, parent_vel.dir);
    registry.emplace<Color>(child, 255, 255, 255);
    registry.emplace<Size>(child, 8.0f);
    registry.emplace<CreatureEnergy>(child, 10.0f, 20.0f);
    registry.emplace<Creature>(child);
    registry.emplace<TimeOf>(child, time, time); 
    registry.emplace<VisionSensors>(child, 0.0f, 0.0f);
    registry.emplace<Bite>(child, 5);
    registry.emplace<Brain>(child, parent_brain.clone());
    return child;
}
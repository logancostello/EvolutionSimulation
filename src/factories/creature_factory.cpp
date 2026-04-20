#include "factories/creature_factory.h"
#include "components/components.h"
#include "random.h"
#include <cmath>

CreatureFactory::CreatureFactory(entt::registry& registry) 
    : registry(registry)
    , brain_factory(registry)
{};

void CreatureFactory::spawn_random(int world_size_x, int world_size_y) {

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
    registry.emplace<CreatureEnergy>(creature, 20, 20.0f);
    registry.emplace<Creature>(creature);
    registry.emplace<VisionSensors>(creature, 0, 0);
    registry.emplace<Bite>(creature, 5);
    registry.emplace<Stomach>(creature, 0, 20);
    registry.emplace<ChildEnergy>(creature, 0, 20.0f);
    registry.emplace<Age>(creature, 0);
    registry.emplace<TimerFreq>(creature, 5, 60);
    
    brain_factory.create_basic_brain(creature);
}


entt::entity CreatureFactory::spawn_child(entt::entity parent) {
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
    registry.emplace<CreatureEnergy>(child, 20.0f, 20.0f);
    registry.emplace<Creature>(child);
    registry.emplace<VisionSensors>(child, 0.0f, 0.0f);
    registry.emplace<Bite>(child, 5);
    registry.emplace<Stomach>(child, 0, 20);
    registry.emplace<ChildEnergy>(child, 0, 20.0f);
    registry.emplace<Brain>(child, parent_brain.clone());
    registry.emplace<Age>(child, 0);
    registry.emplace<TimerFreq>(child, 5, 60);

    return child;
}
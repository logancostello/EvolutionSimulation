#include "systems/sensor_system.h"
#include "components/components.h"
#include <cmath>
#include <limits>

const int VIEW_DISTANCE = 500;

SensorSystem::SensorSystem(entt::registry& registry) : registry(registry) {};

void SensorSystem::set_direction(Position& c_pos, Velocity& vel, entt::entity target, entt::registry& registry, float& dist_out, float& dir_out) {
    if (target == entt::null) {
        dist_out = VIEW_DISTANCE;
        dir_out = 0;
        return;
    }

    Position& t_pos = registry.get<Position>(target);
    float dx = t_pos.x - c_pos.x;
    float dy = t_pos.y - c_pos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    float relative_angle = std::atan2(dy, dx) - vel.dir;
    while (relative_angle >  M_PI) relative_angle -= 2 * M_PI;
    while (relative_angle < -M_PI) relative_angle += 2 * M_PI;

    dist_out = dist;
    dir_out = relative_angle;
}

void SensorSystem::update(QuadTree& lookup_tree) {
    auto creature_view = registry.view<Creature, Position, VisionSensors, Velocity>();

    for (auto [c_entity, c_pos, vision, vel] : creature_view.each()) {
        entt::entity closest_plant = lookup_tree.query_closest(c_entity, c_pos.x, c_pos.y, VIEW_DISTANCE, EntityTag::Plant);
        entt::entity closest_creature = lookup_tree.query_closest(c_entity, c_pos.x, c_pos.y, VIEW_DISTANCE, EntityTag::Creature);

        set_direction(c_pos, vel, closest_plant, registry, vision.dist_to_food, vision.dir_to_food);
        set_direction(c_pos, vel, closest_creature, registry, vision.dist_to_creature, vision.dir_to_creature);
    }
}
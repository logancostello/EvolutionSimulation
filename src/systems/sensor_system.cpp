#include "systems/sensor_system.h"
#include "components/components.h"
#include <cmath>
#include <limits>

const int VIEW_DISTANCE = 500;

SensorSystem::SensorSystem(entt::registry& registry) : registry(registry) {};

void SensorSystem::update(QuadTree& lookup_tree) {
    auto creature_view = registry.view<Creature, Position, VisionSensors, Velocity>();

    for (auto [c_entity, c_pos, vision, vel] : creature_view.each()) {

        // We know this is a plant since only plants are in the tree
        // Eventually this will not be true and query_closest will need to accept a filter
        entt::entity closest_plant = lookup_tree.query_closest(c_pos.x, c_pos.y, VIEW_DISTANCE);

        if (closest_plant == entt::null) {
            vision.dist_to_food = VIEW_DISTANCE;
            vision.dir_to_food = 0;
        } else {

            Position p_pos = registry.get<Position>(closest_plant);

            float dx = p_pos.x - c_pos.x;
            float dy = p_pos.y - c_pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            float world_angle = std::atan2(dy, dx);
            float relative_angle = world_angle - vel.dir;

            // Normalize to [-pi, pi]
            while (relative_angle >  M_PI) relative_angle -= 2 * M_PI;
            while (relative_angle < -M_PI) relative_angle += 2 * M_PI;

            vision.dist_to_food = dist;
            vision.dir_to_food = relative_angle;
        }
    }
}
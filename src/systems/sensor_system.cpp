#include "systems/sensor_system.h"
#include "components/components.h"
#include <cmath>
#include <limits>

SensorSystem::SensorSystem(entt::registry& registry) : registry(registry) {};

void SensorSystem::update() {
    auto creature_view = registry.view<Creature, Position, VisionSensors, Velocity>();
    auto food_view = registry.view<Plant, Position>();

    for (auto [c_entity, c_pos, vision, vel] : creature_view.each()) {
        float closest_dist = std::numeric_limits<float>::max();
        float closest_rads = 0.0f;

        for (auto [p_entity, p_pos] : food_view.each()) {
            float dx = p_pos.x - c_pos.x;
            float dy = p_pos.y - c_pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < closest_dist) {
                closest_dist = dist;

                // Angle to food relative to creature's heading
                float world_angle = std::atan2(dy, dx);
                float relative_angle = world_angle - vel.dir;

                // Normalize to [-pi, pi]
                while (relative_angle >  M_PI) relative_angle -= 2 * M_PI;
                while (relative_angle < -M_PI) relative_angle += 2 * M_PI;

                closest_rads = relative_angle;
            }
        }

        vision.dist_to_food = closest_dist;
        vision.dir_to_food = closest_rads;
    }
}
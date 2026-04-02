#include "systems/sensor_system.h"
#include "components/components.h"
#include <cmath>
#include <limits>

const int VIEW_DISTANCE = 500;

SensorSystem::SensorSystem(entt::registry& registry) : registry(registry) {};

void SensorSystem::update(QuadTree& lookup_tree) {
    auto creature_view = registry.view<Creature, Position, VisionSensors, Velocity>();

    for (auto [c_entity, c_pos, vision, vel] : creature_view.each()) {
        float closest_dist = std::numeric_limits<float>::max();
        float closest_rads = 0.0f;

        auto nearby = std::vector<entt::entity>{};
        lookup_tree.query(c_pos.x, c_pos.y, VIEW_DISTANCE, nearby);

        for (entt::entity e : nearby) {

            if (!registry.all_of<Plant>(e)) continue;

            auto& p_pos = registry.get<Position>(e);

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
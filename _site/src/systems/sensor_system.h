#pragma once
#include <entt/entt.hpp>
#include "quad_tree.h"

class SensorSystem {
    public:
        SensorSystem(entt::registry& registry);
        void update(QuadTree& lookup_tree);

    private:
        entt::registry& registry;
        void set_direction(Position& c_pos, Velocity& vel, entt::entity target, entt::registry& registry, float& dist_out, float& dir_out);
};
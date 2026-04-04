#pragma once
#include <entt/entt.hpp>
#include "quad_tree.h"

class CollisionSystem {
    public:
        CollisionSystem(entt::registry& registry);
        void update(QuadTree& lookup_tree);
    
    private:
        entt::registry& registry;
};
#pragma once
#include <entt/entt.hpp>
#include "quad_tree.h"

class CollisionSystem {
    public:
        CollisionSystem(entt::registry& registry);
        void update(QuadTree& lookup_tree, float dt);
    
    private:
        entt::registry& registry;
        void handle_eating(entt::entity creature, entt::entity plant, float dt);
};
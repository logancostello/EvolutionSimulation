#pragma once
#include <entt/entt.hpp>
#include "quad_tree.h"

class EatingSystem {
    public:
        EatingSystem(entt::registry& registry);
        void update(QuadTree& lookup_tree);
    
    private:
        entt::registry& registry;
};
#include <entt/entt.hpp>
#include "quad_tree.h"

class SpatialIndexSystem {
    public:
        SpatialIndexSystem(entt::registry& registry, QuadTree& lookup_tree);
        void update();
        
    private:
        entt::registry& registry;
        QuadTree& lookup_tree;
};
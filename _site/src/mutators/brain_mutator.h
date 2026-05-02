#pragma once
#include <entt/entt.hpp>
#include "components/brain/brain.h"

class BrainMutator {
    public:
        BrainMutator(entt::registry& registry);
        void mutate(entt::entity entity);   

    private:
        entt::registry& registry;
        
        template<typename T>
        void mutate_nodes(std::vector<T>& nodes);

};
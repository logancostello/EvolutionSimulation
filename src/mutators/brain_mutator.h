#pragma once
#include <entt/entt.hpp>

class BrainMutator {
    public:
        BrainMutator(entt::registry& registry);
        void mutate(entt::entity entity);   

    private:
        entt::registry& registry;

};
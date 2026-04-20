#pragma once
#include <entt/entt.hpp>

class GenomeMutator {
    public:
        GenomeMutator(entt::registry& registry);
        void mutate(entt::entity entity);

    private:
        entt::registry& registry;
        void mutate_size(entt::entity entity);

};
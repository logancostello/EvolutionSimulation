#pragma once
#include <entt/entt.hpp>

class GenomeMutator {
    public:
        GenomeMutator(entt::registry& registry);

    private:
        entt::registry& registry;

};
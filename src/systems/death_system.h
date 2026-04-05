#include <entt/entt.hpp>
#include "factories/carcass_factory.h"

class DeathSystem {
    public:
        DeathSystem(entt::registry& registry, CarcassFactory& carcass_factory);
        void update();

    private:
        entt::registry& registry;
        CarcassFactory& carcass_factory;
};
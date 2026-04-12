#include <entt/entt.hpp>
#include "factories/creature_factory.h"

class EnvironmentSystem {
    public:
        EnvironmentSystem(entt::registry& registry, CreatureFactory& creature_factory);
        void update(float dt, float x, float y, float time);

    private:
        entt::registry& registry;
        CreatureFactory& creature_factory;
        float time_since_immigration = 0;
};
#include <entt/entt.hpp>
#include "factories/creature_factory.h"

class Simulation {
    public:
        Simulation(entt::registry& registry);
        void initialize();
        void update(float dt);

    private:
        entt::registry& registry;
        CreatureFactory creature_factory;
};

#include <entt/entt.hpp>
#include "factories/creature_factory.h"
#include "factories/plant_factory.h"
#include "systems/movement_system.h"

class Simulation {
    public:
        Simulation(entt::registry& registry);
        void initialize();
        void update(float dt);

    private:
        CreatureFactory creature_factory;
        PlantFactory plant_factory;
        MovementSystem movement_system;
};

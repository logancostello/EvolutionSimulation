#pragma once
#include <entt/entt.hpp>
#include "factories/creature_factory.h"
#include "factories/plant_factory.h"
#include "systems/movement_system.h"
#include "systems/metabolism_system.h"
#include "systems/cleanup_system.h"
#include "systems/eating_system.h"
#include "systems/reproduction_system.h"
#include "systems/thinking_system.h"

class Simulation {
    public:
        Simulation(entt::registry& registry);
        void initialize();
        void update(float dt);

    private:
        float time;
        int world_size_x;
        int world_size_y;
        CreatureFactory creature_factory;
        PlantFactory plant_factory;
        MovementSystem movement_system;
        MetabolismSystem metabolism_system;
        CleanupSystem cleanup_system;
        EatingSystem eating_system;
        ReproductionSystem reproduction_system;
        ThinkingSystem thinking_system;
};

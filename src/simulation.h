#pragma once
#include <entt/entt.hpp>
#include "factories/creature_factory.h"
#include "factories/plant_factory.h"
#include "factories/carcass_factory.h"
#include "systems/movement_system.h"
#include "systems/metabolism_system.h"
#include "systems/death_system.h"
#include "systems/collision_system.h"
#include "systems/reproduction_system.h"
#include "systems/thinking_system.h"
#include "systems/sensor_system.h"
#include "mutators/brain_mutator.h"
#include "systems/plant_system.h"
#include "quad_tree.h"
#include "systems/spatial_index_system.h"
#include "systems/environment_system.h"
#include "systems/digestion_system.h"

class Simulation {
    public:
        Simulation(entt::registry& registry);
        void initialize();
        void update(float dt);

    private:
        float time;
        int world_size_x;
        int world_size_y;
        QuadTree entity_lookup_tree;
        SpatialIndexSystem spatial_index_system;
        CreatureFactory creature_factory;
        PlantFactory plant_factory;
        CarcassFactory carcass_factory;
        BrainMutator brain_mutator;
        MovementSystem movement_system;
        MetabolismSystem metabolism_system;
        DeathSystem death_system;
        CollisionSystem collision_system;
        ReproductionSystem reproduction_system;
        ThinkingSystem thinking_system;
        SensorSystem sensor_system;
        PlantSystem plant_system;
        EnvironmentSystem environment_system;
        DigestionSystem digestion_system;
};

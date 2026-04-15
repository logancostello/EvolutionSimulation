#pragma once
#include <entt/entt.hpp>
#include "factories/plant_factory.h"

class PlantSystem {

    public:
        PlantSystem(PlantFactory& plant_factory, entt::registry& registry) : plant_factory(plant_factory), registry(registry) {};
        void update(float dt, int world_size_x, int world_size_y);

    private:
        float accumulated_time = 0;
        PlantFactory& plant_factory;
        entt::registry& registry;

};
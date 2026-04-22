#include "factories/carcass_factory.h"
#include "components/components.h"

CarcassFactory::CarcassFactory(entt::registry& registry) : registry(registry) {};

void CarcassFactory::spawn_carcass(entt::entity creature) {
    
    Position creature_pos = registry.get<Position>(creature);
    Size creature_size = registry.get<Size>(creature);

    entt::entity carcass = registry.create();
    registry.emplace<Position>(carcass, creature_pos);
    registry.emplace<OldPosition>(carcass);
    registry.emplace<Size>(carcass, creature_size);
    registry.emplace<Color>(carcass, 160, 0, 0);
    registry.emplace<FoodEnergy>(carcass, 50);
    registry.emplace<Carcass>(carcass);
}
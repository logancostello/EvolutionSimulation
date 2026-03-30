#include "reproduction_system.h"
#include "components.h"

const float TIME_FOR_REPRODUCTION = 12.5;

ReproductionSystem::ReproductionSystem(entt::registry& registry, CreatureFactory& creature_factory) 
    : registry(registry) 
    , creature_factory(creature_factory)
{};

void ReproductionSystem::update(float time) {
    auto view = registry.view<Creature, TimeOf>();

    for (auto [entity, time_of] : view.each()) {

        if (time - time_of.last_reproduction > TIME_FOR_REPRODUCTION) {
            time_of.last_reproduction = time;
            creature_factory.spawn_child(entity, time);
        }
    }
}
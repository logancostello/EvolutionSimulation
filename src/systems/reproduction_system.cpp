#include "reproduction_system.h"
#include "components/components.h"

const float TIME_FOR_REPRODUCTION = 12.5;

ReproductionSystem::ReproductionSystem(entt::registry& registry, CreatureFactory& creature_factory) 
    : registry(registry) 
    , creature_factory(creature_factory)
{};

void ReproductionSystem::update(float time) {
    auto view = registry.view<Creature, TimeOf>();

    std::vector<entt::entity> to_reproduce;
    for (auto [entity, time_of] : view.each()) {
        if (time - time_of.last_reproduction > TIME_FOR_REPRODUCTION) {
            time_of.last_reproduction = time;
            to_reproduce.push_back(entity);
        }
    }

    for (auto entity : to_reproduce) {
        creature_factory.spawn_child(entity, time);
    }
}

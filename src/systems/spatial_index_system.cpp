#include "spatial_index_system.h"

SpatialIndexSystem::SpatialIndexSystem(entt::registry& registry, QuadTree& lookup_tree) 
    : registry(registry)
    , lookup_tree(lookup_tree)
{};

void SpatialIndexSystem::update() {

    auto dirty_view = registry.view<OldPosition, Position>();
    for (auto [entity, old_pos, pos] : dirty_view.each()) {
        lookup_tree.update_bounds(pos.x, pos.y);
        registry.remove<OldPosition>(entity);
    }

    lookup_tree.reset();

    auto plant_view = registry.view<Position, Plant>();
    for (auto [entity, pos] : plant_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, EntityTag::Plant);
    }

    auto creature_view = registry.view<Position, Creature>();
    for (auto [entity, pos] : creature_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, EntityTag::Creature);
    }

    auto carcass_view = registry.view<Position, Carcass>();
    for (auto [entity, pos] : carcass_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, EntityTag::Carcass);
    }
};
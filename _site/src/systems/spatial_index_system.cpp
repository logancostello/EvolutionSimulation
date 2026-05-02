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

    auto plant_view = registry.view<Position, Size, Plant>();
    for (auto [entity, pos, size] : plant_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, size.radius, EntityTag::Plant);
    }

    auto creature_view = registry.view<Position, Size, Creature>();
    for (auto [entity, pos, size] : creature_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, size.radius, EntityTag::Creature);
    }

    auto carcass_view = registry.view<Position, Size, Carcass>();
    for (auto [entity, pos, size] : carcass_view.each()) {
        lookup_tree.insert(0, entity, pos.x, pos.y, size.radius, EntityTag::Carcass);
    }
};
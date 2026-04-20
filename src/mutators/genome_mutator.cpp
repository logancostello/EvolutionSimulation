#include "mutators/genome_mutator.h"
#include "random.h"
#include "components/components.h"

const float PROB_MUTATE_DEFAULT = 0.1;

GenomeMutator::GenomeMutator(entt::registry& registry) : registry(registry) {};

void GenomeMutator::mutate(entt::entity entity) {
    if (Random::float_range() < PROB_MUTATE_DEFAULT) mutate_size(entity);
}

void GenomeMutator::mutate_size(entt::entity entity) {
    float offset = Random::normal(0, 1);
    registry.get<Size>(entity).radius += offset;
}
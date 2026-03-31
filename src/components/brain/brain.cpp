#include "components/brain/brain.h"

void Brain::load_inputs(entt::registry& registry, entt::entity& entity) {
    for (InputNode input : input_nodes) {
        input.load_input(registry, entity);
    }
}

void Brain::populate_outputs(entt::registry& registry, entt::entity& entity) {
    for (OutputNode output : output_nodes) {
        output.populate_output(registry, entity);
    }
}

void Brain::think() {
    return;
}
#include "mutators/brain_mutator.h"
#include "components/brain/brain.h"
#include "random.h"
#include <algorithm>

const float PROB_WEIGHT_FLIP = 0.05;
const float PROB_WEIGHT_CHANGE = 0.3;
const float PROB_BIAS_CHANGE = 0.3;
const float PROB_TAU_CHANGE = 0.3;
const float PROB_NEW_EDGE = 0.1;
const float PROB_REMOVE_EDGE = 0.1;
const float PROB_NEW_NODE = 0.05;
const float PROB_REMOVE_NODE = 0.05;

const float WEIGHT_OFFSET_MEAN = 0;
const float WEIGHT_OFFSET_STD = 0.5;
const float BIAS_OFFSET_MEAN = 0;
const float BIAS_OFFSET_STD = 0.5;
const float TAU_OFFSET_MEAN = 0;
const float TAU_OFFSET_STD = 0.15;

BrainMutator::BrainMutator(entt::registry& registry) : registry(registry) {};

void BrainMutator::mutate(entt::entity entity) {

    Brain& brain = registry.get<Brain>(entity);

    for (Edge& edge : brain.edges) {
        if (Random::float_range() < PROB_WEIGHT_CHANGE) {
            edge.weight += Random::normal(WEIGHT_OFFSET_MEAN, WEIGHT_OFFSET_STD);
        }

        if (Random::float_range() < PROB_WEIGHT_FLIP) {
            edge.weight *= 1;
        }
    }

    for (Node& node : brain.hidden_nodes) {
        if (Random::float_range() < PROB_BIAS_CHANGE) {
            node.bias += Random::normal(BIAS_OFFSET_MEAN, BIAS_OFFSET_STD);
        }

        if (Random::float_range() < PROB_TAU_CHANGE) {
            node.tau_rise += Random::normal(TAU_OFFSET_MEAN, TAU_OFFSET_STD);
            node.tau_rise = std::max(node.tau_rise, 0.0001f);
        }

        if (Random::float_range() < PROB_TAU_CHANGE) {
            node.tau_fall += Random::normal(TAU_OFFSET_MEAN, TAU_OFFSET_STD);
            node.tau_fall = std::max(node.tau_fall, 0.0001f);
        }
    }

    for (Node& node : brain.output_nodes) {
        if (Random::float_range() < PROB_BIAS_CHANGE) {
            node.bias += Random::normal(BIAS_OFFSET_MEAN, BIAS_OFFSET_STD);
        }
    }

    if (Random::float_range() < PROB_NEW_EDGE) {
        brain.add_random_edge();
    }

    if (Random::float_range() < PROB_REMOVE_EDGE) {
        brain.remove_random_edge();
    }

    if (Random::float_range() < PROB_NEW_NODE) {
        brain.add_random_node();
    }

    if (Random::float_range() < PROB_REMOVE_NODE) {
        brain.remove_random_node();
    }
}

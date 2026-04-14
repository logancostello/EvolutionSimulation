#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

std::vector<InputNode> BrainFactory::get_basic_inputs() {
    return {
        InputNode(0, InputSource::EnergyRatio),
        InputNode(1, InputSource::DistToFood),
        InputNode(2, InputSource::DirToFood),
        InputNode(3, InputSource::DistToCreature),
        InputNode(4, InputSource::DirToCreature),
        InputNode(5, InputSource::DistToCarcass),
        InputNode(6, InputSource::DirToCarcass)
    };
};

std::vector<OutputNode> BrainFactory::get_basic_outputs() {
    return {
        OutputNode(7, OutputSource::VelocityMag, ActivationRange::ZeroToOne),
        OutputNode(8, OutputSource::VelocityTurnRate, ActivationRange::NegOneToOne)
    };
};

void BrainFactory::create_basic_brain(entt::entity& entity) {

    std::vector<InputNode> input_nodes = get_basic_inputs();
    std::vector<OutputNode> output_nodes = get_basic_outputs();
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {};

    registry.emplace<Brain>(entity, input_nodes, output_nodes, hidden_nodes, edges);

    Brain& brain = registry.get<Brain>(entity);

    for (int i = 0; i < Random::int_range(3, 6); i++) {
        brain.add_random_edge();
    }
};
#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

std::vector<InputNode> BrainFactory::get_basic_inputs() {
    return {
        InputNode(0, InputSource::EnergyRatio),
        InputNode(1, InputSource::Fullness),
        InputNode(2, InputSource::ReproductionRatio),
        InputNode(3, InputSource::DistToFood),
        InputNode(4, InputSource::DirToFood),
        InputNode(5, InputSource::DistToCreature),
        InputNode(6, InputSource::DirToCreature),
        InputNode(7, InputSource::DistToCarcass),
        InputNode(8, InputSource::DirToCarcass),
        InputNode(9, InputSource::TimerCycleShort),
        InputNode(10, InputSource::TimerCycleLong),
        InputNode(11, InputSource::TimerManual),
        InputNode(12, InputSource::StateA),
        InputNode(13, InputSource::StateB)
    };
};

std::vector<OutputNode> BrainFactory::get_basic_outputs() {
    return {
        OutputNode(14, OutputSource::VelocityMag, ActivationRange::ZeroToOne),
        OutputNode(15, OutputSource::VelocityTurnRate, ActivationRange::NegOneToOne),
        OutputNode(16, OutputSource::TimerReset, ActivationRange::ZeroToOne),
        OutputNode(17, OutputSource::StateAToggle, ActivationRange::ZeroToOne),
        OutputNode(18, OutputSource::StateBToggle, ActivationRange::ZeroToOne)
    };
};

void BrainFactory::create_random_brain(entt::entity& entity) {

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

void BrainFactory::create_herbivore_brain(entt::entity& entity) {

    std::vector<InputNode> input_nodes = get_basic_inputs();
    std::vector<OutputNode> output_nodes = get_basic_outputs();
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {};

    registry.emplace<Brain>(entity, input_nodes, output_nodes, hidden_nodes, edges);

    Brain& brain = registry.get<Brain>(entity);

    // Add edge from DirToFood to VelocityTurnRate
    brain.edges.push_back(Edge(4, 15, 1.0f));

    // Bias VelocityMag node to always move forward
    for (OutputNode& node : brain.output_nodes) {
        if (node.id == 14) {
            node.bias = 0.1f;
            break;
        }
    }

    // Add some random mutations on top
    for (int i = 0; i < Random::int_range(0, 5); i++) {
        brain.add_random_edge();
    }
};
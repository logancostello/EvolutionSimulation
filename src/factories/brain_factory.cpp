#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

std::vector<InputNode> BrainFactory::get_basic_inputs() {
    return {
        InputNode(0, 0, InputSource::Energy),
        InputNode(1, 0, InputSource::DistToFood),
        InputNode(2, 0, InputSource::DirToFood)
    };
};

std::vector<OutputNode> BrainFactory::get_basic_outputs() {
    return {
        OutputNode(3, 0, OutputSource::VelocityMag),
        OutputNode(4, 0, OutputSource::VelocityDir)
    };
};

void BrainFactory::create_basic_brain(entt::entity& entity) {

    std::vector<InputNode> input_nodes = get_basic_inputs();
    std::vector<OutputNode> output_nodes = get_basic_outputs();
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {Edge(0, 3, 5), Edge(0, 4, 5), Edge(1, 3, 5), Edge(1, 4, 5), Edge(2, 3, 5), Edge(2, 4, 5)};

    registry.emplace<Brain>(entity, input_nodes.size(), output_nodes.size(), input_nodes, output_nodes, hidden_nodes, edges);
};
#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

// Basic for now just to get outputs going
void BrainFactory::create_basic_brain(entt::entity& entity) {

    InputNode input1 = InputNode(0, 0, InputSource::Energy);

    OutputNode output1 = OutputNode(1, 0, OutputSource::VelocityX);
    OutputNode output2 = OutputNode(2, 0, OutputSource::VelocityY);

    std::vector<InputNode> input_nodes = {input1};
    std::vector<OutputNode> output_nodes = {output1, output2};
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {Edge(0, 2, 5)};

    registry.emplace<Brain>(entity, input_nodes, output_nodes, hidden_nodes, edges);
};
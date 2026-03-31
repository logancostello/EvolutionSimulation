#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

// Basic for now just to get outputs going
void BrainFactory::create_basic_brain(entt::entity& entity) {

    InputNode input1 = InputNode(0, 0, InputSource::Energy);
    InputNode input2 = InputNode(1, 0, InputSource::Energy);
    InputNode input3 = InputNode(2, 0, InputSource::Energy);

    OutputNode output1 = OutputNode(3, 0, OutputSource::VelocityMag);
    OutputNode output2 = OutputNode(4, 0, OutputSource::VelocityDir);

    std::vector<InputNode> input_nodes = {input1, input2, input3};
    std::vector<OutputNode> output_nodes = {output1, output2};
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {Edge(0, 3, 5), Edge(0, 4, 5), Edge(1, 3, 5), Edge(1, 4, 5), Edge(2, 3, 5), Edge(2, 4, 5)};

    registry.emplace<Brain>(entity, input_nodes, output_nodes, hidden_nodes, edges);
};
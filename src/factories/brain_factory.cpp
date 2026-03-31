#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

// Basic for now just to get outputs going
void BrainFactory::create_basic_brain(entt::entity& entity) {

    OutputNode output1 = OutputNode(1, 50.0f, 0.0f, OutputSource::VelocityX);
    OutputNode output2 = OutputNode(2, 0.0f, 0.0f, OutputSource::VelocityY);

    int num_inputs = 0;
    int num_outputs = 2;
    std::vector<InputNode> input_nodes = {};
    std::vector<OutputNode> output_nodes = {output1, output2};
    std::vector<Node> hidden_nodes = {};
    std::unordered_map<int, Edge> edges = {};

    registry.emplace<Brain>(entity, num_inputs, num_outputs, input_nodes, output_nodes, hidden_nodes, edges);
};



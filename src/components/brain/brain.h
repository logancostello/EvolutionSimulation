#include <vector>
#include "components/brain/node.h"
#include <unordered_map>

struct Edge {
    int to_node;
    float weight;
};

struct Brain {
    int num_inputs;
    int num_outputs;
    std::vector<InputNode> input_nodes;
    std::vector<OutputNode> output_nodes;
    std::vector<Node> hidden_nodes;
    std::unordered_map<int, Edge> edges;

    void load_inputs(entt::registry& registry, entt::entity& entity);
    void populate_outputs(entt::registry& registry, entt::entity& entity);
    void think();
};


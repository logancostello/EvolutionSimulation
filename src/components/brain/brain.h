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
    std::vector<Node> input_nodes;
    std::vector<Node> output_nodes;
    std::vector<Node> hidden_nodes;
    std::unordered_map<int, Edge> edges;

    void think();
};


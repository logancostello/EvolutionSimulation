#include <vector>
#include "components/brain/node.h"
#include <unordered_map>

struct Edge {
    int from_node;
    int to_node;
    float weight;

    Edge(int from_node, int to_node, float weight) : from_node(from_node), to_node(to_node), weight(weight) {};
};

struct Brain {
    std::vector<InputNode> input_nodes;
    std::vector<OutputNode> output_nodes;
    std::vector<Node> hidden_nodes;
    std::vector<Edge> edges;

    void load_inputs(entt::registry& registry, entt::entity& entity);
    void populate_outputs(entt::registry& registry, entt::entity& entity);
    void think();

    private:
        Node& get_node(int id);
        void set_buffer_to_bias();
        void apply_weights();
        void activate_nodes();
        void apply_buffer();
};


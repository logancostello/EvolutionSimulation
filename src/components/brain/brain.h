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

    Brain() : input_nodes({}), output_nodes({}), hidden_nodes({}), edges({}) {next_node_id = input_nodes.size() + output_nodes.size();};

    Brain(std::vector<InputNode> input_nodes, std::vector<OutputNode> output_nodes, std::vector<Node> hidden_nodes, std::vector<Edge> edges) 
        : input_nodes(input_nodes), output_nodes(output_nodes), hidden_nodes(hidden_nodes), edges(edges)
    {next_node_id = input_nodes.size() + output_nodes.size();};

    Brain clone() const;
    void load_inputs(entt::registry& registry, entt::entity& entity);
    void populate_outputs(entt::registry& registry, entt::entity& entity);
    void think(float dt);

    void add_random_edge();
    void remove_random_edge();
    void add_random_node();
    void remove_random_node();

    private:
        int next_node_id;
        Node& get_node(int id);
        void set_buffer_to_bias();
        void apply_weights();
        void update_nodes(float dt);
        int get_random_non_output_node();
        int get_random_non_input_node();
};


#pragma once
#include <vector>
#include "components/brain/node.h"

enum class NodeType {
    Input, 
    Output,
    Hidden
};

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
    std::unordered_map<int, std::pair<NodeType, int>> node_idx_map;
    std::vector<Edge> edges;

    Brain() : input_nodes({}), output_nodes({}), hidden_nodes({}), edges({}) {next_node_id = input_nodes.size() + output_nodes.size();};

    Brain(std::vector<InputNode> input_nodes, std::vector<OutputNode> output_nodes, std::vector<Node> hidden_nodes, std::vector<Edge> edges) 
        : input_nodes(input_nodes), output_nodes(output_nodes), hidden_nodes(hidden_nodes), edges(edges)
    {next_node_id = input_nodes.size() + output_nodes.size();};

    Brain clone() const;
    void populate_outputs(entt::registry& registry, entt::entity& entity);
    void think(float dt, entt::registry& registry, entt::entity& entity);

    void add_input_node(InputSource source);
    void add_output_node(OutputSource source, ActivationRange range);
    int add_hidden_node();
    void remove_node(int id);
    void add_random_edge();
    void remove_random_edge();
    void swap_random_edge();
    void add_random_unconnected_node();
    void add_random_connected_node();
    void remove_random_node();

    private:
        int next_node_id;
        Node& get_node(int id);
        void reset_next_values();
        void apply_weights();
        void update_nodes(float dt);
        int get_random_node();
        void load_inputs(entt::registry& registry, entt::entity& entity, float dt);
};


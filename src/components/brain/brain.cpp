#include "components/brain/brain.h"
#include "random.h"

void Brain::add_input_node(InputSource source) {
    InputNode node = InputNode(next_node_id++, source);
    node_idx_map.emplace(node.id, std::make_pair(NodeType::Input, input_nodes.size()));
    input_nodes.push_back(node);
}

void Brain::add_output_node(OutputSource source, ActivationRange range) {
    OutputNode node = OutputNode(next_node_id++, source, range);
    node_idx_map.emplace(node.id, std::make_pair(NodeType::Output, output_nodes.size()));
    output_nodes.push_back(node);
}

int Brain::add_hidden_node() {
    Node node = Node(next_node_id++);
    node_idx_map.emplace(node.id, std::make_pair(NodeType::Hidden, hidden_nodes.size()));
    hidden_nodes.push_back(node);
    return node.id;
}

void Brain::remove_node(int id) {
    auto it = node_idx_map.find(id);
    if (it == node_idx_map.end()) return;

    auto [node_type, node_idx] = it->second;

    auto& vec = (node_type == NodeType::Input)  ? (std::vector<Node>&)input_nodes
              : (node_type == NodeType::Output) ? (std::vector<Node>&)output_nodes
              :                                   hidden_nodes;

    int last_id = vec.back().id;

    // Swap target with last, then pop
    // Update the idx of the moved node
    std::swap(vec[node_idx], vec.back());
    node_idx_map[last_id].second = node_idx;

    vec.pop_back();
    node_idx_map.erase(it);
}


void Brain::load_inputs(entt::registry& registry, entt::entity& entity, float dt) {
    for (InputNode& input : input_nodes) {
        input.load_input(registry, entity, dt);
    }
}

void Brain::populate_outputs(entt::registry& registry, entt::entity& entity) {
    for (OutputNode& output : output_nodes) {
        output.populate_output(registry, entity);
    }
}

Node& Brain::get_node(int id) {
    auto iter = node_idx_map.find(id);
    if (iter == node_idx_map.end())
        throw std::runtime_error("Node not found: " + std::to_string(id));

    auto [type, idx] = iter->second;
    switch (type) {
        case NodeType::Input: return input_nodes[idx];
        case NodeType::Output: return output_nodes[idx];
        case NodeType::Hidden: return hidden_nodes[idx];
    }
}

void Brain::think(float dt, entt::registry& registry, entt::entity& entity) {
    reset_next_values();
    load_inputs(registry, entity, dt);
    apply_weights();
    update_nodes(dt);
}

void Brain::reset_next_values() {
    for (Node& n : input_nodes) n.reset_next_value();
    for (Node& n : output_nodes) n.reset_next_value();
    for (Node& n : hidden_nodes) n.reset_next_value();
}

void Brain::apply_weights() {
    for (Edge& edge : edges) {
        Node& from_node = get_node(edge.from_node);
        Node& to_node = get_node(edge.to_node);
        to_node.accept_input(from_node.value * edge.weight);
    }
}

void Brain::update_nodes(float dt) {
    for (Node& n : input_nodes) n.update(dt);
    for (Node& n : output_nodes) n.update(dt);
    for (Node& n : hidden_nodes) n.update(dt);
}

void Brain::add_random_edge() {
    for (int i = 0; i < 10; i++) {
        int from_node = get_random_node();
        int to_node = get_random_node();
        float weight = Random::float_range(-3, 3);

        if (from_node == to_node) continue;;

        // Check edge doesn't already exist
        bool exists = false;
        for (Edge& edge : edges) {
            if (edge.from_node == from_node && edge.to_node == to_node) {
                exists = true;
                break;
            }
        }

        if (exists) continue;

        edges.push_back(Edge(from_node, to_node, weight));
        return;
    }
}

int Brain::get_random_node() {
    float num_nodes = input_nodes.size() + hidden_nodes.size() + output_nodes.size();
    float input_weight = float(input_nodes.size()) / num_nodes;
    float output_weight = float(output_nodes.size()) / num_nodes;

    float rand = Random::float_range();
    if (rand < input_weight) {
        float idx = Random::int_range(0, input_nodes.size() - 1);
        return input_nodes[idx].id;
    } else if (rand < input_weight + output_weight) {
        float idx = Random::int_range(0, output_nodes.size() - 1);
        return output_nodes[idx].id;    
    } else {
        float idx = Random::int_range(0, hidden_nodes.size() - 1);
        return hidden_nodes[idx].id;     
    } 
}

void Brain::remove_random_edge() {
    if (edges.empty()) return;
    int index = Random::int_range(0, edges.size() - 1);
    edges.erase(edges.begin() + index);
}

void Brain::swap_random_edge() {
    for (int i = 0; i < 10; i++) {
        if (edges.empty()) return;
        int edge_idx = Random::int_range(0, edges.size() - 1);
        Edge edge = edges[edge_idx];
        int id = get_random_node();

        if (id == edge.to_node || id == edge.from_node) continue;

        edges.erase(edges.begin() + edge_idx);
        if (Random::float_range() < 0.5) {
            edges.push_back(Edge(id, edge.to_node, edge.weight));
        } else {
            edges.push_back(Edge(edge.from_node, id, edge.weight));
        }
    }
}

void Brain::add_random_unconnected_node() {
    add_hidden_node();
}

void Brain::add_random_connected_node() {
    if (edges.empty()) return;
    int index = Random::int_range(0, edges.size() - 1);
    Edge edge = edges[index];

    int id = add_hidden_node();

    if (Random::float_range() < 0.5) {
        edges.push_back(Edge(edge.from_node, id, edge.weight));
        edges.push_back(Edge(id, edge.to_node, 1));
    } else {
        edges.push_back(Edge(edge.from_node, id, 1));
        edges.push_back(Edge(id, edge.to_node, edge.weight));
    }

    edges.erase(edges.begin() + index);
};

void Brain::remove_random_node() {
    if (hidden_nodes.empty()) return;

    int index = Random::int_range(0, hidden_nodes.size() - 1);
    int remove_id = hidden_nodes[index].id;

    std::vector<int> in_nodes;
    std::vector<int> out_nodes;

    for (Edge& edge : edges) {
        if (edge.from_node == remove_id) out_nodes.push_back(edge.to_node);
        if (edge.to_node   == remove_id) in_nodes.push_back(edge.from_node);
    }

    for (int in_id : in_nodes) {
        for (int out_id : out_nodes) {
            bool exists = false;
            for (Edge& edge : edges) {
                if (edge.from_node == in_id && edge.to_node == out_id) {
                    exists = true;
                    break;
                }
            }
            if (exists) continue;

            float weight;
            if (Random::float_range() < 0.5f) {
                for (Edge& edge : edges)
                    if (edge.from_node == in_id && edge.to_node == remove_id)
                        weight = edge.weight;
            } else {
                for (Edge& edge : edges)
                    if (edge.from_node == remove_id && edge.to_node == out_id)
                        weight = edge.weight;
            }

            edges.push_back(Edge(in_id, out_id, weight));
        }
    }

    edges.erase(
        std::remove_if(edges.begin(), edges.end(), [remove_id](const Edge& edge) {
            return edge.from_node == remove_id || edge.to_node == remove_id;
        }),
        edges.end()
    );

    remove_node(remove_id);
}


Brain Brain::clone() const {
    Brain copy;
    copy.input_nodes = input_nodes;  
    copy.output_nodes = output_nodes; 
    copy.hidden_nodes = hidden_nodes;
    copy.edges = edges;
    copy.next_node_id = next_node_id;
    copy.node_idx_map = node_idx_map;
    return copy;
}
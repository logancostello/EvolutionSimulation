#include "components/brain/brain.h"
#include "random.h"

void Brain::load_inputs(entt::registry& registry, entt::entity& entity) {
    for (InputNode& input : input_nodes) {
        input.load_input(registry, entity);
    }
}

void Brain::populate_outputs(entt::registry& registry, entt::entity& entity) {
    for (OutputNode& output : output_nodes) {
        output.populate_output(registry, entity);
    }
}

// Naive for now, improved in future
Node& Brain::get_node(int id) {
    for (InputNode& node : input_nodes) {
        if (node.id == id) {
            return node;
        }
    }

    for (OutputNode& node : output_nodes) {
        if (node.id == id) {
            return node;
        }
    }

    for (Node& node : hidden_nodes) {
        if (node.id == id) {
            return node;
        }
    }

    throw std::runtime_error("Node not found: " + std::to_string(id));
}

void Brain::think(float dt, entt::registry& registry, entt::entity& entity) {
    reset_next_values();
    load_inputs(registry, entity);
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

        if (from_node == to_node) break;

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

void Brain::add_random_unconnected_node() {
    Node node = Node(next_node_id++);
    hidden_nodes.push_back(node);
}

void Brain::add_random_connected_node() {
    if (edges.empty()) return;
    int index = Random::int_range(0, edges.size() - 1);
    Edge edge = edges[index];

    Node node = Node(next_node_id++);
    hidden_nodes.push_back(node);

    if (Random::float_range() < 0.5) {
        edges.push_back(Edge(edge.from_node, node.id, edge.weight));
        edges.push_back(Edge(node.id, edge.to_node, 1));
    } else {
        edges.push_back(Edge(edge.from_node, node.id, 1));
        edges.push_back(Edge(node.id, edge.to_node, edge.weight));
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

    hidden_nodes.erase(hidden_nodes.begin() + index);
}


Brain Brain::clone() const {
    Brain copy;
    copy.input_nodes = input_nodes;  
    copy.output_nodes = output_nodes; 
    copy.hidden_nodes = hidden_nodes;
    copy.edges = edges;
    copy.next_node_id = next_node_id;
    return copy;
}
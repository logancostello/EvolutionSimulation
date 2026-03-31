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

void Brain::think() {
    set_buffer_to_bias();
    apply_weights();
    activate_nodes();
    apply_buffer();
}

void Brain::set_buffer_to_bias() {
    for (OutputNode& node : output_nodes) {
        node.next_value = node.bias;
    }

    for (Node& node : hidden_nodes) {
        node.next_value = node.bias;
    }
}

void Brain::apply_weights() {
    for (Edge& edge : edges) {
        get_node(edge.to_node).next_value += get_node(edge.from_node).value * edge.weight;
    }
}

void Brain::activate_nodes() {
    for (OutputNode& node : output_nodes) {
        node.activate();
    }

    for (Node& node : hidden_nodes) {
        node.activate();
    }
}

void Brain::apply_buffer() {
    for (OutputNode& node : output_nodes) {
        node.value = node.next_value;
    }

    for (Node& node : hidden_nodes) {
        node.value = node.next_value;
    }
}

void Brain::add_random_connection() {
    for (int i = 0; i < 10; i++) {
        int from_node = get_random_non_output_node();
        int to_node = get_random_non_input_node();
        float weight = Random::float_range(-3, 3);

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

int Brain::get_random_non_output_node() {

    float input_prob = input_nodes.size() / (input_nodes.size() + hidden_nodes.size());

    if (Random::float_range() < input_prob) {
        int index = Random::int_range(0, input_nodes.size() - 1);
        return input_nodes[index].id;
    } else {
        int index = Random::int_range(0, hidden_nodes.size() - 1);
        return hidden_nodes[index].id;
    }
}

int Brain::get_random_non_input_node() {

    float output_prob = float(output_nodes.size()) / float(output_nodes.size() + hidden_nodes.size());

    if (Random::float_range() < output_prob) {
        int index = Random::int_range(0, output_nodes.size() - 1);
        return output_nodes[index].id;
    } else {
        int index = Random::int_range(0, hidden_nodes.size() - 1);
        return hidden_nodes[index].id;
    }
}
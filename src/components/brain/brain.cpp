#include "components/brain/brain.h"

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
#pragma once
#include <entt/entt.hpp>

struct Node {
    int id;
    float value;
    float bias;

    Node(int id, float value, float bias) : id(id), value(value), bias(bias) {}

    float activate();    
};

struct InputNode : Node {
    void load_input(entt::registry& registry, entt::entity& entity);
};

enum class OutputSource {
    VelocityX,
    VelocityY
};

struct OutputNode: Node {
    OutputSource output_source;

    OutputNode(int id, float value, float bias, OutputSource output_source)
        : Node(id, value, bias), output_source(output_source) {}

    void populate_output(entt::registry& registry, entt::entity& entity);
};
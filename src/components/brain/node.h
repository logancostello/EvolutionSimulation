#pragma once
#include <entt/entt.hpp>

struct Node {
    int id;
    float value;
    float bias;

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

    void populate_output(entt::registry& registry, entt::entity& entity);
};
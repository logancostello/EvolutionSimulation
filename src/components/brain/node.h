#pragma once
#include <entt/entt.hpp>
#include "random.h"

struct Node {
    int id;
    float value;
    float next_value;
    float bias;
    float tau_rise;
    float tau_fall;

    Node(int id, float bias) : id(id), value(0), next_value(0), bias(bias), tau_rise(0.05), tau_fall(0.05) {};

    void update(float dt);
    float activate();    
};

enum class InputSource {
    Energy,
    DistToFood,
    DirToFood
};

struct InputNode : Node {
    InputSource input_source;

    InputNode(int id, float bias, InputSource input_source)
        : Node(id, bias), input_source(input_source) {}

    void load_input(entt::registry& registry, entt::entity& entity);
};

enum class OutputSource {
    VelocityMag,
    VelocityTurnRate
};

struct OutputNode: Node {
    OutputSource output_source;

    OutputNode(int id, float bias, OutputSource output_source)
        : Node(id, bias), output_source(output_source) {}

    void populate_output(entt::registry& registry, entt::entity& entity);
};
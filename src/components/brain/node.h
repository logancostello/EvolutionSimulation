#pragma once
#include <entt/entt.hpp>
#include "random.h"

enum class InputSource {
    Energy,
    DistToFood,
    DirToFood
};

enum class OutputSource {
    VelocityMag,
    VelocityTurnRate
};

enum class ActivationFunc {
    Tanh,
    Sigmoid
};

enum class ActivationRange {
    NegOneToOne,
    ZeroToOne
};

ActivationFunc pick_random_activation_func();
ActivationFunc pick_random_activation_func(ActivationRange range);

struct Node {
    int id;
    float value;
    float next_value;
    float bias;
    float tau_rise;
    float tau_fall;
    ActivationFunc activation_func;

    Node(int id, float bias) 
        : id(id)
        , value(0)
        , next_value(0)
        , bias(bias)
        , tau_rise(0.05)
        , tau_fall(0.05)
        , activation_func(pick_random_activation_func())
    {};

    Node(int id, float bias, ActivationFunc activation_func) 
        : id(id)
        , value(0)
        , next_value(0)
        , bias(bias)
        , tau_rise(0.05)
        , tau_fall(0.05)
        , activation_func(activation_func)
    {};

    void update(float dt);
    float activate();   
};

struct InputNode : Node {
    InputSource input_source;

    InputNode(int id, float bias, InputSource input_source)
        : Node(id, bias), input_source(input_source) {}

    void load_input(entt::registry& registry, entt::entity& entity);
};

struct OutputNode: Node {
    OutputSource output_source;
    ActivationRange output_range;

    OutputNode(int id, float bias, OutputSource output_source, ActivationRange output_range)
        : Node(id, bias, pick_random_activation_func(output_range)), output_source(output_source), output_range(output_range) {}

    void populate_output(entt::registry& registry, entt::entity& entity);
};
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
    Sigmoid,
    Sign,
    Step,
    ReLU,
    Sine,
    Identity,
    Absolute,
    Softsign,
    SineMult,
    SineSquared,
    Quantize
};

enum class ActivationRange {
    Any,
    NegOneToOne,
    ZeroToOne,
    NonNegative,
    Unbounded
};

ActivationFunc pick_random_activation_func(ActivationRange range);

struct Node {
    int id;
    float value;
    float next_value;
    float bias;
    float tau_rise;
    float tau_fall;
    ActivationRange activation_range;
    ActivationFunc activation_func;

    Node(int id) 
        : id(id)
        , value(0)
        , next_value(0)
        , bias(Random::normal(0, 0.5))
        , tau_rise(0.05)
        , tau_fall(0.05)
        , activation_range(ActivationRange::Any)
        , activation_func(pick_random_activation_func(ActivationRange::Any))
    {};

    Node(int id, ActivationRange activation_range) 
        : id(id)
        , value(0)
        , next_value(0)
        , bias(Random::normal(0, 0.5))
        , tau_rise(0.05)
        , tau_fall(0.05)
        , activation_range(activation_range)
        , activation_func(pick_random_activation_func(activation_range))
    {};

    void update(float dt);
    float activate();   
};

struct InputNode : Node {
    InputSource input_source;

    InputNode(int id, InputSource input_source)
        : Node(id), input_source(input_source) {}

    void load_input(entt::registry& registry, entt::entity& entity);
};

struct OutputNode: Node {
    OutputSource output_source;

    OutputNode(int id, OutputSource output_source, ActivationRange activation_range)
        : Node(id, activation_range), output_source(output_source) {}

    void populate_output(entt::registry& registry, entt::entity& entity);
};
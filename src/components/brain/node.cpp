#include "components/brain/node.h"
#include "components/components.h"
#include <cmath>

const std::vector<ActivationFunc> all_activation_funcs = {
    ActivationFunc::Tanh,
    ActivationFunc::Sigmoid
};

const std::unordered_map<ActivationRange, std::vector<ActivationFunc>> funcs_by_range = {
    { ActivationRange::NegOneToOne, { ActivationFunc::Tanh } },
    { ActivationRange::ZeroToOne,   { ActivationFunc::Sigmoid } },
};

ActivationFunc pick_random_activation_func() {
    int index = Random::int_range(0, all_activation_funcs.size() - 1);
    return all_activation_funcs[index];
}

ActivationFunc pick_random_activation_func(ActivationRange range) {
    const auto& options = funcs_by_range.at(range);
    int index = Random::int_range(0, options.size() - 1);
    return options[index];
}

float Node::activate() {
    switch(activation_func) {
        case ActivationFunc::Tanh:
            return std::tanh(next_value);
        case ActivationFunc::Sigmoid:
            return 1.0 / (1.0 + std::exp(-next_value));
    }
}

void Node::update(float dt) {
    next_value += bias;
    float target = activate();
    float tau = (target > value) ? tau_rise : tau_fall;
    float d_value = (-value + target) / tau;
    value = value + d_value * dt;
}

void InputNode::load_input(entt::registry& registry, entt::entity& entity) {
    switch (input_source) {
        case InputSource::Energy:
            next_value = registry.get<Energy>(entity).energy;
            break;
        case InputSource::DistToFood:
            next_value = registry.get<VisionSensors>(entity).dist_to_food;
            break;
        case InputSource::DirToFood:
            next_value = registry.get<VisionSensors>(entity).dir_to_food;
            break;
    }
};

void OutputNode::populate_output(entt::registry& registry, entt::entity& entity) {
    switch (output_source) {
        case OutputSource::VelocityMag:
            registry.get<Velocity>(entity).mag = value * 100; 
            break;
        case OutputSource::VelocityTurnRate:
            registry.get<Velocity>(entity).turn_rate = value * 3.14; 
            break;
    }
};
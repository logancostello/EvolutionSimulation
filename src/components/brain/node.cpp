#include "components/brain/node.h"
#include "components/components.h"
#include <cmath>

// For now just tanh. Many possible activation functions in future
float Node::activate() {
    return std::tanh(next_value);
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
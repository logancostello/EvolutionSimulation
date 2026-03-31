#include "components/brain/node.h"
#include "components/components.h"
#include <cmath>

// For now just tanh. Many possible activation functions in future
float Node::activate() {
    return std::tanh(next_value);
}

void InputNode::load_input(entt::registry& registry, entt::entity& entity) {
    switch (input_source) {
        case InputSource::Energy:
            value = registry.get<Energy>(entity).energy;
            break;
    }
};

void OutputNode::populate_output(entt::registry& registry, entt::entity& entity) {
    switch (output_source) {
        case OutputSource::VelocityMag:
            registry.get<Velocity>(entity).mag = value; 
            break;
        case OutputSource::VelocityDir:
            registry.get<Velocity>(entity).dir = value; 
            break;
    }
};
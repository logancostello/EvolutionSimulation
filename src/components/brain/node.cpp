#include "components/brain/node.h"
#include "components/components.h"
#include <cmath>

// For now just tanh. Many possible activation functions in future
float Node::activate() {
    return std::tanh(value);
}

// For now just constant input, eventually this will get a corresponding value from the components
void InputNode::load_input(entt::registry& registry, entt::entity& entity) {
    value = 1.0f;
};

void OutputNode::populate_output(entt::registry& registry, entt::entity& entity) {
    switch (output_source) {
        case OutputSource::VelocityX:
            registry.replace<Velocity>(entity).dx = value; 
            break;
        case OutputSource::VelocityY:
            registry.replace<Velocity>(entity).dy = value; 
            break;
    }
};
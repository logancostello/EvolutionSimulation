#include "components/brain/node.h"
#include <cmath>

// For now just tanh. Many possible activation functions in future
float Node::activate() {
    return std::tanh(value);
}
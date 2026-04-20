#include "components/brain/node.h"
#include "components/components.h"
#include <cmath>

const std::vector<ActivationFunc> all_activation_funcs = {
    ActivationFunc::Tanh,
    ActivationFunc::Sigmoid,
    ActivationFunc::Sign,
    ActivationFunc::Step,
    ActivationFunc::ReLU,
    ActivationFunc::Sine,
    ActivationFunc::Identity,
    ActivationFunc::Absolute,
    ActivationFunc::Softsign,
    ActivationFunc::SineMult,
    ActivationFunc::SineSquared,
    ActivationFunc::Quantize
};

const std::vector<Aggregator> all_aggregators = {
    Aggregator::Sum,
    Aggregator::Product,
    Aggregator::Min,
    Aggregator::Max,
    Aggregator::NumPos,
    Aggregator::NumNeg
};

const std::unordered_map<ActivationRange, std::vector<ActivationFunc>> funcs_by_range = {
    { ActivationRange::NegOneToOne, { ActivationFunc::Tanh, ActivationFunc::Sign, ActivationFunc::Sine, ActivationFunc::Softsign } },
    { ActivationRange::ZeroToOne,   { ActivationFunc::Sigmoid, ActivationFunc::Step } },
    { ActivationRange::NonNegative, { ActivationFunc::ReLU, ActivationFunc::Absolute } },
    { ActivationRange::Unbounded,   { ActivationFunc::Identity, ActivationFunc::SineMult, ActivationFunc::SineSquared, ActivationFunc::Quantize } },
};

float Node::activate() {
    switch(activation_func) {
        case ActivationFunc::Tanh:
            return std::tanh(next_value);
        case ActivationFunc::Sigmoid:
            return 1.0f / (1.0f + std::exp(-next_value));
        case ActivationFunc::Sign:
            return (next_value > 0) - (next_value < 0);
        case ActivationFunc::Step:
            return next_value >= 0 ? 1.0f : 0.0f;
        case ActivationFunc::ReLU:
            return std::max(0.0f, next_value);
        case ActivationFunc::Sine:
            return std::sin(next_value);
        case ActivationFunc::Identity:
            return next_value;
        case ActivationFunc::Absolute:
            return std::abs(next_value);
        case ActivationFunc::Softsign:
            return next_value / (1.0f + std::abs(next_value));
        case ActivationFunc::SineMult:
            return next_value * std::sin(next_value);
        case ActivationFunc::SineSquared:
            return std::sin(next_value * next_value);
        case ActivationFunc::Quantize:
            return std::floor(next_value);
    }
}

ActivationFunc pick_random_activation_func(ActivationRange range) {
    if (range == ActivationRange::Any) {
        int index = Random::int_range(0, all_activation_funcs.size() - 1);
        return all_activation_funcs[index];
    }

    const auto& options = funcs_by_range.at(range);
    int index = Random::int_range(0, options.size() - 1);
    return options[index];
}

Aggregator pick_random_aggregator() {
    int index = Random::int_range(0, all_aggregators.size() - 1);
    return all_aggregators[index];
}

void Node::update(float dt) {
    next_value += bias;
    float target = activate();
    float tau = (target > value) ? tau_rise : tau_fall;
    float d_value = (-value + target) / tau;
    value = value + d_value * dt;
}

void Node::reset_next_value() {
    switch(aggregator) {
        case Aggregator::Sum:
            next_value = 0;
            break;
        case Aggregator::Product:
            next_value = 1;
            break;
        case Aggregator::Min:
            next_value = std::numeric_limits<float>::max();
            break;
        case Aggregator::Max:
            next_value = std::numeric_limits<float>::min();
            break;
        case Aggregator::NumPos:
            next_value = 0;
            break;
        case Aggregator::NumNeg:
            next_value = 0;
            break;
    } 
}

void Node::accept_input(float input) {
    switch(aggregator) {
        case Aggregator::Sum:
            next_value += input;
            break;
        case Aggregator::Product:
            next_value *= input;
            break;
        case Aggregator::Min:
            next_value = std::min(next_value, input);
            break;
        case Aggregator::Max:
            next_value = std::max(next_value, input);
            break;
        case Aggregator::NumPos:
            if (input > 0) next_value += 1;
            break;
        case Aggregator::NumNeg:
            if (input < 0) next_value += 1;
            break;
    }
}

void InputNode::load_input(entt::registry& registry, entt::entity& entity, float dt) {
    switch (input_source) {
        case InputSource::EnergyRatio: {
            CreatureEnergy& e = registry.get<CreatureEnergy>(entity);
            next_value = e.energy / e.max;
            break;
        }
        case InputSource::Fullness: {
            Stomach& stomach = registry.get<Stomach>(entity);
            next_value = stomach.potential_energy / stomach.max;
            break;
        }
        case InputSource::ReproductionRatio: {
            ChildEnergy& child_energy = registry.get<ChildEnergy>(entity);
            next_value = child_energy.energy;
            break;
        }
        case InputSource::DistToFood:
            next_value = registry.get<VisionSensors>(entity).dist_to_food;
            break;
        case InputSource::DirToFood:
            next_value = registry.get<VisionSensors>(entity).dir_to_food;
            break;
        case InputSource::DistToCreature:
            next_value = registry.get<VisionSensors>(entity).dist_to_creature;
            break;
        case InputSource::DirToCreature:
            next_value = registry.get<VisionSensors>(entity).dir_to_creature;
            break;
        case InputSource::DistToCarcass:
            next_value = registry.get<VisionSensors>(entity).dist_to_carcass;
            break;
        case InputSource::DirToCarcass:
            next_value = registry.get<VisionSensors>(entity).dir_to_carcass;
            break;
        case InputSource::TimerCycleShort: {
            float age = registry.get<Age>(entity).age;
            float freq = registry.get<BrainTimer>(entity).short_freq;
            next_value = std::sin(age * freq);
            break;
        }
        case InputSource::TimerCycleLong: {
            float age = registry.get<Age>(entity).age;
            float freq = registry.get<BrainTimer>(entity).long_freq;
            next_value = std::sin(age * freq);
            break;
        }
        case InputSource::TimerManual: {
            BrainTimer& timer = registry.get<BrainTimer>(entity);
            timer.manual_time += dt;
            next_value = timer.manual_time;
        }
    }
};

void OutputNode::populate_output(entt::registry& registry, entt::entity& entity) {
    switch (output_source) {
        case OutputSource::VelocityMag:
            registry.get<Velocity>(entity).mag = value; 
            break;
        case OutputSource::VelocityTurnRate:
            registry.get<Velocity>(entity).turn_rate = value; 
            break;
        case OutputSource::TimerReset: 
            if (value > 0.5) registry.get<BrainTimer>(entity).manual_time = 0;
            break;
    }
};
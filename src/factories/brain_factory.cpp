#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

void BrainFactory::add_basic_inputs(Brain& brain) {
    brain.add_input_node(InputSource::EnergyRatio);
    brain.add_input_node(InputSource::Fullness);
    brain.add_input_node(InputSource::ReproductionRatio);
    brain.add_input_node(InputSource::DistToFood);
    brain.add_input_node(InputSource::DirToFood);
    brain.add_input_node(InputSource::DistToCreature);
    brain.add_input_node(InputSource::DirToCreature);
    brain.add_input_node(InputSource::DistToCarcass);
    brain.add_input_node(InputSource::DirToCarcass);
    brain.add_input_node(InputSource::TimerCycleShort);
    brain.add_input_node(InputSource::TimerCycleLong);
    brain.add_input_node(InputSource::TimerManual);
    brain.add_input_node(InputSource::StateA);
    brain.add_input_node(InputSource::StateB);
}

void BrainFactory::add_basic_outputs(Brain& brain) {
    brain.add_output_node(OutputSource::VelocityMag, ActivationRange::ZeroToOne);
    brain.add_output_node(OutputSource::VelocityTurnRate, ActivationRange::NegOneToOne);
    brain.add_output_node(OutputSource::TimerReset, ActivationRange::ZeroToOne);
    brain.add_output_node(OutputSource::StateAToggle, ActivationRange::ZeroToOne);
    brain.add_output_node(OutputSource::StateBToggle, ActivationRange::ZeroToOne);
}

void BrainFactory::create_random_brain(entt::entity& entity) {

    registry.emplace<Brain>(entity);

    Brain& brain = registry.get<Brain>(entity);
    add_basic_inputs(brain);
    add_basic_outputs(brain);

    for (int i = 0; i < Random::int_range(3, 6); i++) {
        brain.add_random_edge();
    }
};

void BrainFactory::create_herbivore_brain(entt::entity& entity) {

    registry.emplace<Brain>(entity);
    
    Brain& brain = registry.get<Brain>(entity);
    add_basic_inputs(brain);
    add_basic_outputs(brain);
    
    // Add edge from DirToFood to VelocityTurnRate
    brain.edges.push_back(Edge(4, 15, 1.0f));

    // Bias VelocityMag node to always move forward
    for (OutputNode& node : brain.output_nodes) {
        if (node.id == 14) {
            node.bias = 0.1f;
            break;
        }
    }

    // Add some random mutations on top
    for (int i = 0; i < Random::int_range(0, 5); i++) {
        brain.add_random_edge();
    }
};
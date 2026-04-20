#include "factories/brain_factory.h"
#include "components/components.h"

BrainFactory::BrainFactory(entt::registry& registry) : registry(registry) {};

std::vector<InputNode> BrainFactory::get_basic_inputs() {
    return {
        InputNode(0, InputSource::EnergyRatio),
        InputNode(1, InputSource::Fullness),
        InputNode(2, InputSource::ReproductionRatio),
        InputNode(3, InputSource::DistToFood),
        InputNode(4, InputSource::DirToFood),
        InputNode(5, InputSource::DistToCreature),
        InputNode(6, InputSource::DirToCreature),
        InputNode(7, InputSource::DistToCarcass),
        InputNode(8, InputSource::DirToCarcass),
        InputNode(9, InputSource::TimerCycleShort),
        InputNode(10, InputSource::TimerCycleLong),
        InputNode(11, InputSource::TimerManual)
    };
};

std::vector<OutputNode> BrainFactory::get_basic_outputs() {
    return {
        OutputNode(12, OutputSource::VelocityMag, ActivationRange::ZeroToOne),
        OutputNode(13, OutputSource::VelocityTurnRate, ActivationRange::NegOneToOne),
        OutputNode(14, OutputSource::TimerReset, ActivationRange::ZeroToOne)
    };
};

void BrainFactory::create_basic_brain(entt::entity& entity) {

    std::vector<InputNode> input_nodes = get_basic_inputs();
    std::vector<OutputNode> output_nodes = get_basic_outputs();
    std::vector<Node> hidden_nodes = {};
    std::vector<Edge> edges = {};

    registry.emplace<Brain>(entity, input_nodes, output_nodes, hidden_nodes, edges);

    Brain& brain = registry.get<Brain>(entity);

    for (int i = 0; i < Random::int_range(3, 6); i++) {
        brain.add_random_edge();
    }
};
#include <entt/entt.hpp>
#include "components/brain/node.h"

class BrainFactory {
    public:
        BrainFactory(entt::registry& registry);
        void create_random_brain(entt::entity& entity);
        void create_herbivore_brain(entt::entity& entity);


    private:
        entt::registry& registry;
        std::vector<InputNode> get_basic_inputs();
        std::vector<OutputNode> get_basic_outputs();
};
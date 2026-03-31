#include <entt/entt.hpp>
#include "components/brain/node.h"

class BrainFactory {
    public:
        BrainFactory(entt::registry& registry);
        void create_basic_brain(entt::entity& entity);


    private:
        entt::registry& registry;
        std::vector<InputNode> get_basic_inputs();
        std::vector<OutputNode> get_basic_outputs();
};
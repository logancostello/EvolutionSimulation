#include <entt/entt.hpp>
#include "components/brain/node.h"
#include "components/brain/brain.h"

class BrainFactory {
    public:
        BrainFactory(entt::registry& registry);
        void create_random_brain(entt::entity& entity);
        void create_herbivore_brain(entt::entity& entity);


    private:
        entt::registry& registry;
        void add_basic_inputs(Brain& brain);
        void add_basic_outputs(Brain& brain);
};
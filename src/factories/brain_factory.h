#include <entt/entt.hpp>

class BrainFactory {
    public:
        BrainFactory(entt::registry& registry);
        void create_basic_brain(entt::entity& entity);

    private:
        entt::registry& registry;
};
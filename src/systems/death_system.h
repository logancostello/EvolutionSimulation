#include <entt/entt.hpp>

class DeathSystem {
    public:
        DeathSystem(entt::registry& registry);
        void update();

    private:
        entt::registry& registry;
};
#include <entt/entt.hpp>

class CleanupSystem {
    public:
        CleanupSystem(entt::registry& registry);
        void update();

    private:
        entt::registry& registry;
};
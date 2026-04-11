#include <entt/entt.hpp>

class EnvironmentSystem {
    public:
        EnvironmentSystem(entt::registry& registry);
        void update(float dt);

    private:
        entt::registry& registry;
};
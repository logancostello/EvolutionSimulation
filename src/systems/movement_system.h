#include <entt/entt.hpp>

class MovementSystem {
    public:
        MovementSystem(entt::registry& registry);
        void update(float dt);

    private:
        entt::registry& registry;
};
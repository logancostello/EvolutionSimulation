#include <entt/entt.hpp>

class MetabolismSystem {
    public:
        MetabolismSystem(entt::registry& registry);
        void update(float dt);

    private:
        entt::registry& registry;
};
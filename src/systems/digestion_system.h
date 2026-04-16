#include <entt/entt.hpp>

class DigestionSystem {

    public:
        DigestionSystem(entt::registry& registry);
        void update(float dt);

    private:
        entt::registry& registry;
};
#include <entt/entt.hpp>

class SensorSystem {
    public:
        SensorSystem(entt::registry& registry);
        void update();

    private:
        entt::registry& registry;
};
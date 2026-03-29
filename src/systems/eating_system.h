#include <entt/entt.hpp>

class EatingSystem {
    public:
        EatingSystem(entt::registry& registry);
        void update();
    
    private:
        entt::registry& registry;
};
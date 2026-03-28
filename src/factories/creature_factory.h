#include <entt/entt.hpp>

class CreatureFactory {
    public:
        CreatureFactory(entt::registry& registry);
        void spawn_random();
        
    private:
        entt::registry& registry;
};
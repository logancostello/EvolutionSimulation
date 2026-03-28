#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Renderer {
    public:
        void draw(sf::RenderWindow& window, entt::registry& registry);
};
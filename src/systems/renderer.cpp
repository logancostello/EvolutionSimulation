#include "systems/renderer.h"
#include "components.h"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

void Renderer::draw(sf::RenderWindow& window, entt::registry& registry) {
    auto view = registry.view<Position, Color, Size>();
    for (auto [entity, pos, color, size] : view.each()) {

        sf::CircleShape circle(size.radius);
        circle.setPosition({pos.x, pos.y});
        circle.setFillColor(sf::Color(color.r, color.g, color.b));
        window.draw(circle);
    }   
}
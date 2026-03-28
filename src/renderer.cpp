#include "renderer.h"
#include "components.h"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

void Renderer::draw(sf::RenderWindow& window, entt::registry& registry) {
    auto view = registry.view<Position>();
    for (auto entity : view) {

        Position pos = view.get<Position>(entity);
        sf::CircleShape circle(5);
        circle.setPosition({pos.x, pos.y});
        circle.setFillColor(sf::Color(255, 255, 255));
        window.draw(circle);
    }   
}
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "simulation.h"
#include "components.h"
#include "systems/renderer.h"

int main() {

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Evolution Simulation"
    );
    window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();

    entt::registry registry;
    Simulation sim = Simulation(registry);
    Renderer renderer = Renderer();

    sim.initialize();

    // Track pan state outside the event loop
    bool panning = false;
    sf::Vector2i last_mouse_pos;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                float zoom_factor = 1.0f - (scroll->delta * 0.1f);
                zoom_factor = std::clamp(zoom_factor, 0.5f, 2.0f);
                view.zoom(zoom_factor);
                window.setView(view);
            }
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    panning = true;
                    last_mouse_pos = sf::Mouse::getPosition(window);
                }
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    panning = false;
                }
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
                if (panning) {
                    sf::Vector2i current_pos = sf::Vector2i(mouse->position.x, mouse->position.y);
                    sf::Vector2i delta = last_mouse_pos - current_pos;

                    // Scale pan speed by current zoom level so it feels consistent
                    float zoom_level = view.getSize().x / window.getSize().x;
                    view.move(sf::Vector2f(delta) * zoom_level);

                    last_mouse_pos = current_pos;
                    window.setView(view);
                }
            }
        }

        window.clear(sf::Color(15, 15, 20));

        float dt = 1 / 60.0f;
        sim.update(dt);

        renderer.draw(window, registry);

        window.display();
    }
}
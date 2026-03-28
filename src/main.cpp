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

    entt::registry registry;
    Simulation sim = Simulation(registry);
    Renderer renderer = Renderer();

    sim.initialize();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
        }

        window.clear(sf::Color(15, 15, 20));

        float dt = 1 / 60.0f;
        sim.update(dt);

        renderer.draw(window, registry);

        window.display();
    }
}
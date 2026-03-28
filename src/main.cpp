#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "simulation.h"
#include "components.h"
#include "renderer.h"

int main() {

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Evolution Simulation"
    );
    window.setFramerateLimit(60);

    entt::registry registry;
    Simulation sim = Simulation();
    Renderer renderer = Renderer();

    // Add testing entity for now
    entt::entity testing_entity = registry.create();
    registry.emplace<Position>(testing_entity, 400.0f, 300.0f);
    registry.emplace<Velocity>(testing_entity, 50.0f, 0.0f);

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
        sim.update(registry, dt);

        renderer.draw(window, registry);

        window.display();
    }
}
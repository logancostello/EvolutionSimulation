#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include "random.h"
#include "simulation.h"
#include "components/components.h"
#include "systems/renderer.h"

int main(int argc, char* argv[]) {

    if (argc > 1) {
        try {
            uint32_t seed = std::stoul(argv[1]);
            Random::seed(seed);
        } catch (const std::exception&) {
            std::cerr << "Invalid seed: " << argv[1] << "\n";
            return 1;
        }
    } else {
        Random::seed();
    }


    sf::RenderWindow window(
        sf::VideoMode({1600, 1000}),
        "Evolution Simulation"
    );

    window.setFramerateLimit(60);

    sf::Vector2f window_size((float)window.getSize().x, (float)window.getSize().y);
    sf::View view(sf::FloatRect(-window_size / 2.f, window_size));
    window.setView(view);

    entt::registry registry;
    auto sim = std::make_unique<Simulation>(registry);
    auto renderer = Renderer();


    float dt = 1 / 60.0f;

    sim->initialize();

    bool panning = false;
    sf::Vector2i last_mouse_pos;

    bool paused = false;
    bool uncapped_frames = false;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (key->code == sf::Keyboard::Key::Space) {
                    paused = !paused;
                } else if (key->code == sf::Keyboard::Key::F) {
                    uncapped_frames = !uncapped_frames;
                    if (uncapped_frames) {
                        window.setFramerateLimit(0);
                    } else {
                        window.setFramerateLimit(60);
                    }
                }
            }
            
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                sf::Vector2f new_size(resized->size);
                view.setSize(new_size);
                window.setView(view);
            }
            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                float zoom_factor = 1.0f - (scroll->delta * 0.1f);
                zoom_factor = std::clamp(zoom_factor, 0.5f, 2.0f);

                // Get mouse position in world space before zooming
                sf::Vector2f mouse_world_before = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                view.zoom(zoom_factor);
                window.setView(view);

                // Get mouse position in world space after zooming
                sf::Vector2f mouse_world_after = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Shift view by the difference so mouse stays fixed
                view.move(mouse_world_before - mouse_world_after);
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

                    float zoom_level = view.getSize().x / window.getSize().x;
                    view.move(sf::Vector2f(delta) * zoom_level);

                    last_mouse_pos = current_pos;
                    window.setView(view);
                }
            }
        }

        window.clear(sf::Color(15, 15, 20));

        if (!paused) sim->update(dt);

        renderer.draw(window, registry);

        window.display();
    }
}
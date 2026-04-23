#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <iomanip>
#include <chrono>
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
    std::cout << "Seed: " << Random::get_seed() << std::endl;

    // -------------------------------------------------------------------------
    // Headless mode: ./main <seed> <ticks>
    //
    // Runs the simulation for exactly <ticks> updates with no window or
    // renderer, then prints timing and a determinism checksum.
    // -------------------------------------------------------------------------
    if (argc > 2) {
        int max_ticks = 0;
        try {
            max_ticks = std::stoi(argv[2]);
        } catch (const std::exception&) {
            std::cerr << "Invalid tick count: " << argv[2] << "\n";
            return 1;
        }

        entt::registry registry;
        auto sim = std::make_unique<Simulation>(registry);
        sim->initialize();

        const float dt = 1.0f / 60.0f;

        auto t_start = std::chrono::steady_clock::now();

        for (int tick = 0; tick < max_ticks; tick++) {
            sim->update(dt);
        }

        auto t_end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(t_end - t_start).count();

        // Determinism checksum — sum of all entity positions.
        // Run twice with the same seed/ticks: checksums must match.
        // If they don't after a refactor, you broke determinism.
        double checksum = 0.0;
        auto view = registry.view<Position>();
        for (auto [entity, pos] : view.each()) {
            checksum += static_cast<double>(pos.x) + static_cast<double>(pos.y);
        }

        std::cout << "Ticks:    " << max_ticks << "\n";
        std::cout << "Elapsed:  " << std::fixed << std::setprecision(3) << elapsed << "s\n";
        std::cout << "Avg/tick: " << std::fixed << std::setprecision(3) << (elapsed / max_ticks * 1000.0) << "ms\n";
        std::cout << "CHECKSUM  " << std::fixed << std::setprecision(4) << checksum << "\n";

        return 0;
    }

    // -------------------------------------------------------------------------
    // Normal windowed mode (unchanged)
    // -------------------------------------------------------------------------
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

    const float dt = 1.0f / 60.0f;

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

                sf::Vector2f mouse_world_before = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                view.zoom(zoom_factor);
                window.setView(view);

                sf::Vector2f mouse_world_after = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
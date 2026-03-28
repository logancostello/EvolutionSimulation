#include <SFML/Graphics.hpp>

int main() {

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Evolution Simulation"
    );
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
        }

        window.clear(sf::Color(15, 15, 20));
        window.display();
    }
}
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa01 - Sfondo");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("../resources/background.png"))
    {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/background.png" << std::endl;
        return -1;
    }

    sf::Sprite background(texture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = texture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    background.setScale({scaleX, scaleY});

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.draw(background);
        window.display();
    }

    return 0;
}
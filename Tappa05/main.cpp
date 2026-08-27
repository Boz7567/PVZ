#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "pvz.h"

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa05 - Refactoring");

int main() {
    srand(time(nullptr));

    if (!loadTextures())
        return -1;

    sf::Vector2u windowSize = window.getSize();
    g_textureSize = backgroundTexture.getSize();
    g_scaleX = static_cast<float>(windowSize.x) / g_textureSize.x;
    g_scaleY = static_cast<float>(windowSize.y) / g_textureSize.y;

    sf::Sprite background(backgroundTexture);
    background.setScale({g_scaleX, g_scaleY});

    sf::Sprite peaIcon(peaIconTexture);
    peaIcon.setPosition({10.0f, 20.0f});
    sf::Sprite sunIcon(sunIconTexture);
    sunIcon.setPosition({10.0f, 80.0f});

    grid = new Plant*[5];
    for (int i = 0; i < 5; ++i) {
        grid[i] = new Plant[9];
    }

    window.setFramerateLimit(60);

    PlantType selection = EMPTY;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (mousePos.x >= sunIcon.getPosition().x &&
                    mousePos.x <= sunIcon.getPosition().x + sunIconTexture.getSize().x * g_scaleX &&
                    mousePos.y >= sunIcon.getPosition().y &&
                    mousePos.y <= sunIcon.getPosition().y + sunIconTexture.getSize().y * g_scaleY) {
                    selection = SUN;   // SUN
                }
                else if (mousePos.x >= peaIcon.getPosition().x &&
                         mousePos.x <= peaIcon.getPosition().x + peaIconTexture.getSize().x * g_scaleX &&
                         mousePos.y >= peaIcon.getPosition().y &&
                         mousePos.y <= peaIcon.getPosition().y + peaIconTexture.getSize().y * g_scaleY) {
                    selection = PEA;   // PEA
                }
                else {
                    // Try placing a plant
                    for (int i = 0; i < 5; ++i) {
                        for (int j = 0; j < 9; ++j) {
                            sf::Vector2f tilePos = indexToPos(i, j);
                            if (mousePos.x >= tilePos.x &&
                                mousePos.x <= tilePos.x + baseTileWidth * g_scaleX &&
                                mousePos.y >= tilePos.y &&
                                mousePos.y <= tilePos.y + baseTileHeight * g_scaleY) {
                                if (grid[i][j].getType() == EMPTY && selection != 0) {
                                    grid[i][j] = Plant(selection);
                                }
                            }
                        }
                    }
                    selection = EMPTY;
                }
            }
        }

        window.draw(background);
        window.draw(peaIcon);
        window.draw(sunIcon);

        if (selection == 1) {
            sf::FloatRect bounds = sunIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }
        else if (selection == 2) {
            sf::FloatRect bounds = peaIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }

        drawGameBoard(window);
        window.display();
    }

    for (int i = 0; i < 5; ++i)
        delete[] grid[i];
    delete[] grid;

    return 0;
}
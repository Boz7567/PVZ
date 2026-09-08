#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "pvz.h"

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa06 - Sun cost");
void sunHandler(std::vector<Sun> &suns, sf::Clock &skySunSpawner);

int main() {
    srand(time(nullptr));
    sf::Clock skySunSpawner;
    if (!loadTextures())
        return -1;

    sf::Font sunValueFont("../resources/Burbank_Big_Condensed_Black_700.ttf");

    sf::Vector2u windowSize = window.getSize();
    g_textureSize = backgroundTexture.getSize();
    g_scaleX = static_cast<float>(windowSize.x) / g_textureSize.x;
    g_scaleY = static_cast<float>(windowSize.y) / g_textureSize.y;

    //background
    sf::Sprite background(backgroundTexture);
    background.setScale({g_scaleX, g_scaleY});

    // icons
    sf::Sprite peaIcon(peaIconTexture);
    peaIcon.setPosition({10.0f, 20.0f});
    sf::Sprite sunIcon(sunIconTexture);
    sunIcon.setPosition({10.0f, 80.0f});

    // suns
    sf::Sprite sunBox(sunBoxTexture);
    sunBox.setPosition({140.0f, 20.0f});

    // number of suns
    int sun = 50;
    auto temp = sunBox.getPosition();
    sf::Text displayText(sunValueFont, std::to_string(sun), 35);
    displayText.setFillColor(sf::Color::Black);
    displayText.setPosition({sunBox.getPosition().x + 90, sunBox.getPosition().y});
    
    // suns (both from sky and from sunflowers)
    std::vector<Sun> suns;
    Sun aux(sunTexture);
    aux.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
    suns.push_back(aux);

    grid = new Plant*[5];
    for (int i = 0; i < 5; ++i) {
        grid[i] = new Plant[9];
    }

    window.setFramerateLimit(60);
    skySunSpawner.start();

    PlantType selection = EMPTY;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                bool aux = false;
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < suns.size(); ++i) {
                    sf::FloatRect bounds = suns[i].sprite.getGlobalBounds(); // simpler & accurate
                    if (bounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                        sun += 25;
                        suns.erase(suns.begin() + i);
                        aux = true;
                        break;
                    }
                }
                if(aux)
                    continue;
                if (mousePos.x >= sunIcon.getPosition().x &&
                    mousePos.x <= sunIcon.getPosition().x + sunIconTexture.getSize().x * g_scaleX &&
                    mousePos.y >= sunIcon.getPosition().y &&
                    mousePos.y <= sunIcon.getPosition().y + sunIconTexture.getSize().y * g_scaleY) {
                    if(sun >= 50){
                        selection = SUN;   // SUN
                    }
                }
                else if (mousePos.x >= peaIcon.getPosition().x &&
                         mousePos.x <= peaIcon.getPosition().x + peaIconTexture.getSize().x * g_scaleX &&
                         mousePos.y >= peaIcon.getPosition().y &&
                         mousePos.y <= peaIcon.getPosition().y + peaIconTexture.getSize().y * g_scaleY) {
                    if(sun >=100){
                        selection = PEA;   // PEA
                    }
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
                                if (grid[i][j].getType() == EMPTY && selection != EMPTY) {
                                    grid[i][j] = Plant(selection);
                                    if(selection == SUN){
                                        sun -=50;
                                        grid[i][j].clock.start();
                                    }
                                    if(selection == PEA){
                                        sun -=100;
                                    }
                                }
                            }
                        }
                    }
                    selection = EMPTY;
                }
            }
        }

        displayText.setString(std::to_string(sun));

        window.draw(background);
        window.draw(peaIcon);
        window.draw(sunIcon);

        if (selection == SUN) {
            sf::FloatRect bounds = sunIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }
        else if (selection == PEA) {
            sf::FloatRect bounds = peaIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }

        window.draw(sunBox);
        window.draw(displayText);

        drawGameBoard(window);
        
        sunHandler(suns, skySunSpawner);

        for(auto s : suns){
            window.draw(s.sprite);
        }


        window.display();
    }

    for (int i = 0; i < 5; ++i)
        delete[] grid[i];
    delete[] grid;

    return 0;
}

void sunHandler(std::vector<Sun> &suns, sf::Clock &skySunSpawner){
    if(skySunSpawner.getElapsedTime().asMilliseconds() >= (6000 + rand()%4001)){
        Sun aux(sunTexture);
        aux.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
        suns.push_back(aux);
        skySunSpawner.restart();
    }

    for(int i = 0; i<5; i++){
        for(int j = 0; j<9; j++){
            if(grid[i][j].getType() == SUN){
                if(grid[i][j].clock.getElapsedTime().asMilliseconds() >= 6000 + rand()%2000){
                    Sun aux(sunTexture);
                    aux.sprite.setPosition({indexToPos(i,j).x + rand()%(Stexture.getSize().x), indexToPos(i,j).y });
                    aux.falling = false;
                    suns.push_back(aux); 
                    grid[i][j].clock.restart();
                }
            }
        }
    }

    for (auto it = suns.begin(); it != suns.end(); ) {
        auto &s = *it;
        if (s.sprite.getPosition().y <= s.stop_coord && s.falling)
            s.sprite.move({0.0f, 2.0f});
        s.sprite.rotate(sf::degrees(2));

        if (s.clock.getElapsedTime().asMilliseconds() >= (6000 + rand() % 2000)) {
            it = suns.erase(it);
        } else {
            ++it;
        }
    }
}
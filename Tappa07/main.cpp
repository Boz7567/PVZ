#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "pvz.h"

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa07 - Better zombies");
void sunHandler(std::vector<Sun> &suns, sf::Clock &skySunSpawner);
void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void zombieHandler(std::vector<Zombie> zombies[5], sf::Clock &zombieSpawner);

int main(){
    srand(time(nullptr));
    sf::Clock skySunSpawner;
    sf::Clock zombieSpawner;
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
    int sun = 100;  // deve essere 50
    auto temp = sunBox.getPosition();
    sf::Text displayText(sunValueFont, std::to_string(sun), 35);
    displayText.setFillColor(sf::Color::Black);
    displayText.setPosition({sunBox.getPosition().x + 90, sunBox.getPosition().y});
    
    // suns (both from sky and from sunflowers)
    std::vector<Sun> suns;
    Sun auxS(sunTexture);
    auxS.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
    suns.push_back(auxS);

    // zombies
    std::vector<Zombie> zombies[5];

    //peas
    std::vector<Pea> peas[5];

    grid = new Plant*[5];
    for (int i = 0; i < 5; ++i) {
        grid[i] = new Plant[9];
    }

    window.setFramerateLimit(60);

    skySunSpawner.start();
    zombieSpawner.start();
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
                                mousePos.y <= tilePos.y + baseTileHeight * g_scaleY){
                                if (grid[i][j].getType() == EMPTY && selection != EMPTY){
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

        sunHandler(suns, skySunSpawner);
        zombieHandler(zombies, zombieSpawner);
        peaHandler(peas, zombies);

        window.draw(sunBox);
        window.draw(displayText);

        drawGameBoard(window);
        
        for(int i = 0; i<5; i++){
            for(auto &z : zombies[i]){
                window.draw(z.sprite);
            }
            for(auto &p : peas[i]){
                window.draw(p.sprite);
            }
        }


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

void zombieHandler(std::vector<Zombie> zombies[5], sf::Clock &zombieSpawner){
    // Spawn logic
    if(zombieSpawner.getElapsedTime().asMilliseconds() >= 10000){
        Zombie aux(zombieTexture);
        aux.sprite.setPosition({1280.f, ((baseStartY + aux.row * baseTileHeight) * g_scaleY) - zombieTexture.getSize().y/2.f});
        zombies[aux.row].push_back(aux);
        zombieSpawner.restart();
    }
    
    for(int row = 0; row < 5; row++){
        for(auto &z : zombies[row]){
            bool hasPlant = false;  // is there a plant in the row?
            sf::Vector2f zombiePos = z.sprite.getPosition();
            
            for(int col = 0; col < 9; col++){
                if(grid[row][col].getType() != EMPTY){
                    sf::Vector2f tilePos = indexToPos(row, col);
                    
                    // Check if the zombie is within attack range of this plant
                    float tileCenterX = tilePos.x + (baseTileWidth * g_scaleX) / 2.f;
                    float distance = zombiePos.x - tileCenterX;
                    float attackRange = 30.f; // Adjust this value as needed
                    
                    if(distance < attackRange){
                        hasPlant = true;
                        // Optionally: attack the plant here
                        grid[row][col].damage(1);
                        if(grid[row][col].isDead()){
                            grid[row][col] = Plant(EMPTY);
                        }
                        break;
                    }
                }
            }
            
            // If no plant nearby, move forward
            if(!hasPlant){
                z.sprite.move({-0.5f, 0});
            }
        }
    }
}

void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]){
    for (int row = 0; row < 5; row++){
        for (auto itP = peas[row].begin(); itP != peas[row].end();){
            bool peaHit = false;
            // Move the pea first
            itP->sprite.move({5.0f, 0.0f}); // or your speed
            // Check collision with zombies in the same row
            for(auto itZ = zombies[row].begin(); itZ != zombies[row].end();){
                sf::FloatRect peaBounds = itP->sprite.getGlobalBounds();
                sf::FloatRect zombieBounds = itZ->sprite.getGlobalBounds();

                // slightly adjust the bounds so that the pea visibily hits the zombie

                // if(peaBounds.findIntersection(zombieBounds).has_value()){
                if(itP->sprite.getPosition().x >= itZ->sprite.getPosition().x + 15.f){
                    // Pea hits the zombie
                    peaHit = true;
                    itZ->health -= 20;

                    // If zombie dies, remove it
                    if(itZ->health <= 0){
                        itZ = zombies[row].erase(itZ);
                    } else{
                        ++itZ;
                    }
                    break; // pea can only hit one zombie
                } else{
                    itZ++;
                }
            }
            // If the pea hit something, remove it
            if(peaHit){
                itP = peas[row].erase(itP);
            } else{
                // Optional: remove pea if it goes off screen
                if(itP->sprite.getPosition().x > 1300.f){
                    itP = peas[row].erase(itP);
                } else{
                    itP++;
                }
            }
        }
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col].getType() == PEA) {
                // Check if there's a zombie ahead
                float plantX = (baseStartX + col * baseTileWidth) * g_scaleX;
                bool zombieAhead = false;
                for (auto &z : zombies[row]) {
                    if (z.sprite.getPosition().x > plantX) {
                        zombieAhead = true;
                        break;
                    }
                }
                // If a zombie exists ahead and cooldown is over, shoot
                if (zombieAhead && grid[row][col].clock.getElapsedTime().asSeconds() > 2.0f) {
                    Pea aux(peaTexture, row);
                    aux.sprite.setPosition({indexToPos(row, col).x + 50.f, indexToPos(row, col).y + 26.f});
                    peas[row].push_back(aux);
                    grid[row][col].clock.restart();
                }
            }
        }
    }
}

void sunHandler(std::vector<Sun> &suns, sf::Clock &skySunSpawner){
    if(skySunSpawner.getElapsedTime().asMilliseconds() >= (8000 + rand()%4001)){
        Sun aux(sunTexture);
        aux.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
        suns.push_back(aux);
        skySunSpawner.restart();
    }

    for(int i = 0; i<5; i++){
        for(int j = 0; j<9; j++){
            if(grid[i][j].getType() == SUN){
                if(grid[i][j].clock.getElapsedTime().asMilliseconds() >= 10000 + rand()%4001){
                    Sun aux(sunTexture);
                    aux.sprite.setPosition({indexToPos(i,j).x + rand()%(Stexture.getSize().x), indexToPos(i,j).y});
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
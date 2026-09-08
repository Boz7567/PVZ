#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "pvz.h"

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa10 - Plants vs Zombies");
void sunHandler(std::vector<Sun> &suns);
void sunSpawner(std::vector<Sun> &suns);
void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void peaSpawner(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void zombieHandler(std::vector<Zombie> zombies[5]);
void zombieSpawner(std::vector<Zombie> zombies[5]);

sf::Time elapsed;
float zombietime = 0;
float suntime = 0;
int lvl = 0;
int zombiesLeft = LVL1;
bool levelUp = false;
float levelUpTimer = 0.0f;
bool gameWon = false;
float winTimer = 0.0f;
bool defeat = false;
float defeatTimer = 0.0f; // NEW

// Check if all zombies are dead
bool allZombiesDead(const std::vector<Zombie> zombies[5]) {
    int total = 0;
    for (int i = 0; i < 5; ++i)
        total += zombies[i].size();
    return total == 0;
}

int main(){
    srand(time(nullptr));
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
    sf::Sprite nutIcon(nutIconTexture);
    nutIcon.setPosition({10.0f, 140.0f});

    // suns box
    sf::Sprite sunBox(sunBoxTexture);
    sunBox.setPosition({140.0f, 20.0f});

    int sun = 50;
    sf::Text displayText(sunValueFont, std::to_string(sun), 35);
    displayText.setFillColor(sf::Color::Black);
    displayText.setPosition({sunBox.getPosition().x + 90.0f, sunBox.getPosition().y});

    // level box
    sf::Sprite lvlBox(lvlBoxTexture);
    lvlBox.setPosition({1280.0f - sunBoxTexture.getSize().x, 20.0f});
    
    lvl = 1;
    sf::Text displayTextLvl(sunValueFont, "1 - " + std::to_string(lvl), 35);
    auto pos = lvlBox.getPosition();
    displayTextLvl.setFillColor(sf::Color::Black);
    displayTextLvl.setPosition({pos.x + 70.0f, pos.y});
    sf::Text LVL(sunValueFont, "LVL", 35);
    LVL.setPosition({pos.x + 7.0f, pos.y});
    LVL.setFillColor(sf::Color::Black);

    // zombies left box
    sf::Sprite zombiesLeftBox(zombieBoxTexture);
    zombiesLeftBox.setPosition({1280.0f - 2 * sunBoxTexture.getSize().x - 10.0f, 20.0f});
    sf::Text displayTextZombiesLeft(sunValueFont, std::to_string(zombiesLeft), 35);
    displayTextZombiesLeft.setFillColor(sf::Color::Black);
    displayTextZombiesLeft.setPosition({zombiesLeftBox.getPosition().x + 90.0f, zombiesLeftBox.getPosition().y});

    // defeat text
    sf::Text defeatText(sunValueFont, "The zombies ate your brain!", 50);
    defeatText.setFillColor(sf::Color::Red);
    sf::FloatRect textBoundsD = defeatText.getLocalBounds();
    defeatText.setPosition({(windowSize.x - textBoundsD.size.x) / 2.f,
                             (windowSize.y - textBoundsD.size.y) / 2.f});
    sf::RectangleShape popupBgD({textBoundsD.size.x + 40.f, textBoundsD.size.y + 40.f});
    popupBgD.setFillColor(sf::Color(0, 0, 0, 200));
    popupBgD.setPosition({defeatText.getPosition().x - 20.f, defeatText.getPosition().y - 20.f});

    // level up text
    sf::Text levelUpText(sunValueFont, "Congratulations! Move to Level 2!", 50);
    levelUpText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = levelUpText.getLocalBounds();
    levelUpText.setPosition({(windowSize.x - textBounds.size.x) / 2.f,
                             (windowSize.y - textBounds.size.y) / 2.f});
    sf::RectangleShape popupBg({textBounds.size.x + 40.f, textBounds.size.y + 40.f});
    popupBg.setFillColor(sf::Color(0, 0, 0, 200));
    popupBg.setPosition({levelUpText.getPosition().x - 20.f, levelUpText.getPosition().y - 20.f});

    // win text
    sf::Text winText(sunValueFont, "YOU WIN!", 70);
    winText.setFillColor(sf::Color::Yellow);
    sf::FloatRect winTextBounds = winText.getLocalBounds();
    winText.setPosition({(windowSize.x - winTextBounds.size.x) / 2.f,
                        (windowSize.y - winTextBounds.size.y) / 2.f});
    sf::RectangleShape winBg({winTextBounds.size.x + 40.f, winTextBounds.size.y + 40.f});
    winBg.setFillColor(sf::Color(0, 0, 0, 200));
    winBg.setPosition({winText.getPosition().x - 20.f, winText.getPosition().y - 20.f});

    // suns
    std::vector<Sun> suns;
    Sun auxS(sunTexture);
    auxS.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
    suns.push_back(auxS);

    // zombies
    std::vector<Zombie> zombies[5];

    //peas
    std::vector<Pea> peas[5];

    // Grid allocation
    grid = new Plant*[5];
    for (int i = 0; i < 5; ++i) {
        grid[i] = new Plant[9];
    }

    window.setFramerateLimit(60);
    PlantType selection = EMPTY;

    sf::Clock clock;
    
    while (window.isOpen()) {
        elapsed = clock.restart();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                bool aux = false;
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < suns.size(); ++i) {
                    sf::FloatRect bounds = suns[i].sprite.getGlobalBounds();
                    if (bounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                        sun += 25;
                        suns.erase(suns.begin() + i);
                        aux = true;
                        break;
                    }
                }
                if(aux)
                    continue;
                
                // Icon selections
                if (mousePos.x >= sunIcon.getPosition().x &&
                    mousePos.x <= sunIcon.getPosition().x + sunIconTexture.getSize().x * g_scaleX &&
                    mousePos.y >= sunIcon.getPosition().y &&
                    mousePos.y <= sunIcon.getPosition().y + sunIconTexture.getSize().y * g_scaleY) {
                    if(sun >= 50){
                        selection = SUN;
                    }
                }
                else if (mousePos.x >= peaIcon.getPosition().x &&
                         mousePos.x <= peaIcon.getPosition().x + peaIconTexture.getSize().x * g_scaleX &&
                         mousePos.y >= peaIcon.getPosition().y &&
                         mousePos.y <= peaIcon.getPosition().y + peaIconTexture.getSize().y * g_scaleY) {
                    if(sun >= 100){
                        selection = PEA;
                    }
                }
                else if (mousePos.x >= nutIcon.getPosition().x &&
                         mousePos.x <= nutIcon.getPosition().x + nutIconTexture.getSize().x * g_scaleX &&
                         mousePos.y >= nutIcon.getPosition().y &&
                         mousePos.y <= nutIcon.getPosition().y + nutIconTexture.getSize().y * g_scaleY) {
                    if(sun >= 50){
                        selection = NUT;
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
                                    sun -= grid[i][j].cost;
                                    grid[i][j].time = 0;
                                }
                            }
                        }
                    }
                    selection = EMPTY;
                }
            }
        }

        displayText.setString(std::to_string(sun));
        displayTextLvl.setString("1 - " + std::to_string(lvl));
        displayTextZombiesLeft.setString(std::to_string(zombiesLeft));

        window.draw(background);
        window.draw(peaIcon);
        window.draw(sunIcon);
        window.draw(nutIcon);

        // Overlay for selected icon
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
        else if (selection == NUT) {
            sf::FloatRect bounds = nutIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }

        // Main game loop (paused on level up, win, and defeat)
        if (!levelUp && !gameWon && !defeat) {
            sunSpawner(suns);
            sunHandler(suns);

            zombieSpawner(zombies);
            zombieHandler(zombies);

            peaSpawner(peas, zombies);
            peaHandler(peas, zombies);
        }

        window.draw(sunBox);
        window.draw(lvlBox);
        window.draw(zombiesLeftBox);
        window.draw(displayText);
        window.draw(displayTextLvl);
        window.draw(LVL);
        window.draw(displayTextZombiesLeft);

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

        // Defeat screen
        if(defeat){
            defeatTimer += elapsed.asSeconds();
            window.draw(popupBgD);
            window.draw(defeatText);
            
            if (defeatTimer >= 6.0f) {
                window.close();
            }
        }

        // Level up popup
        if (levelUp) {
            levelUpTimer += elapsed.asSeconds();
            window.draw(popupBg);
            window.draw(levelUpText);
            suns.clear();
            if (levelUpTimer >= 2.0f) {
                for (int i = 0; i < 5; ++i)
                    delete[] grid[i];
                delete[] grid;
                grid = new Plant*[5];
                for (int i = 0; i < 5; ++i)
                    grid[i] = new Plant[9];

                lvl = 2;
                zombiesLeft = LVL2;

                for (int i = 0; i < 5; ++i) {
                    zombies[i].clear();
                    peas[i].clear();
                }

                sun = 50;
                levelUp = false;
                levelUpTimer = 0.0f;
                zombietime = 0.0f;
                suntime = 0.0f;
            }
        }

        // Win screen
        if (gameWon) {
            winTimer += elapsed.asSeconds();
            window.draw(winBg);
            window.draw(winText);
            
            if (winTimer >= 6.0f) {
                window.close();
            }
        }
        window.display();
    }

    for (int i = 0; i < 5; ++i)
        delete[] grid[i];
    delete[] grid;

    return 0;
}

void zombieSpawner(std::vector<Zombie> zombies[5]) {
    zombietime += elapsed.asSeconds();
    switch(lvl) {
        case 1:
        {
            int spawnDelay = (zombiesLeft < LVL1-(LVL1/3)) ? 5 : 10;
            if(zombietime >= spawnDelay && zombiesLeft > 0) {
                Zombie aux(zombieTexture, zombieEatingTexture, ZOMBIE_SHEET_COLS, ZOMBIE_SHEET_ROWS,
                           ZOMBIE_SWITCH_TIME, rand()%5, BASIC);
                float rowTop = (baseStartY + aux.row * baseTileHeight) * g_scaleY;
                float groundY = rowTop + baseTileHeight * g_scaleY;
                aux.sprite.setPosition({1280.f, groundY});
                zombies[aux.row].push_back(aux);
                zombiesLeft--;
                zombietime = 0;
            }
            break;
        }
        case 2:
            if(zombietime >= 10 && zombiesLeft > 0) {
                ZombieType type = (rand()%4 == 3) ? CONE : BASIC;
                sf::Texture& tex = (type == CONE) ? zombieConeTexture : zombieTexture;
                sf::Texture& Etex = (type == CONE) ? zombieEatingConeTexture : zombieEatingTexture;
                Zombie aux(tex, Etex, ZOMBIE_SHEET_COLS, ZOMBIE_SHEET_ROWS,
                           ZOMBIE_SWITCH_TIME, rand()%5, type);
                float rowTop = (baseStartY + aux.row * baseTileHeight) * g_scaleY;
                float groundY = rowTop + baseTileHeight * g_scaleY;
                aux.sprite.setPosition({1280.f, groundY});
                zombies[aux.row].push_back(aux);
                zombiesLeft--;
                zombietime = 0;
            }
            break;
    }
}

void zombieHandler(std::vector<Zombie> zombies[5]){
    for(int row = 0; row < 5; row++){
        for(auto &z : zombies[row]){
            z.update(elapsed.asSeconds());

            bool hasPlant = false;
            z.eating = false;
            sf::Vector2f zombiePos = z.sprite.getPosition();
            
            for(int col = 0; col < 9; col++){
                if(grid[row][col].getType() != EMPTY){
                    z.eating = false;
                    sf::Vector2f tilePos = indexToPos(row, col);
                    float tileCenterX = tilePos.x + (baseTileWidth * g_scaleX) / 2.f;
                    float distance = zombiePos.x - tileCenterX;
                    float attackRange = 30.f;
                    
                    if(distance < attackRange){
                        hasPlant = true;
                        z.eating = true;
                        z.attackCooldown -= elapsed.asSeconds();
                        if(z.attackCooldown <= 0.0f){
                            grid[row][col].damage(20);
                            z.attackCooldown = 1.0f;
                        }

                        if(grid[row][col].isDead()){
                            grid[row][col] = Plant(EMPTY);
                        }
                        break;
                    }
                }
            }
            
            if(!hasPlant){
                z.sprite.move({-elapsed.asSeconds()*ZOMBIE_SPEED, 0});
                if(z.sprite.getPosition().x < 300.0f){
                    defeat = true; // FIXED: only triggers when off screen
                }
            }
        }
    }
}

void sunSpawner(std::vector<Sun> &suns){
    suntime += elapsed.asSeconds();

    if(suntime >= 8 + rand()%2){
        Sun aux(sunTexture);
        aux.sprite.setPosition({static_cast<float>(rand()%850 + 400), 0.0f});
        suns.push_back(aux);
        suntime = 0;
    }

    for(int i = 0; i<5; i++){
        for(int j = 0; j<9; j++){
            if(grid[i][j].getType() == SUN){
                grid[i][j].time += elapsed.asSeconds();
                if(grid[i][j].time >= 8 + rand()%3){
                    Sun aux(sunTexture);
                    aux.sprite.setPosition({indexToPos(i,j).x + rand()%(Stexture.getSize().x), indexToPos(i,j).y});
                    aux.falling = false;
                    suns.push_back(aux);
                    grid[i][j].time = 0;
                }
            }
        }
    }
}

void sunHandler(std::vector<Sun> &suns){
    for (auto it = suns.begin(); it != suns.end();){
        auto &s = *it;
        s.time += elapsed.asSeconds();
        if (s.sprite.getPosition().y <= s.stop_coord && s.falling)
            s.sprite.move({0, elapsed.asSeconds()*100});
        s.sprite.rotate(sf::degrees(elapsed.asSeconds()*90));

        if (s.time >= 9 + rand()%3){
            it = suns.erase(it);
        } else{
            ++it;
        }
    }
}

void peaSpawner(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]){
    for(int row = 0; row<5; row++){
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col].getType() == PEA) {
                grid[row][col].time += elapsed.asSeconds();

                float plantX = (baseStartX + col * baseTileWidth) * g_scaleX;
                bool zombieAhead = false;
                for (auto &z : zombies[row]) {
                    if (z.sprite.getPosition().x > plantX) {
                        zombieAhead = true;
                        break;
                    }
                }
                if (zombieAhead && grid[row][col].time > 2.0f) {
                    Pea aux(peaTexture, row);
                    aux.sprite.setPosition({indexToPos(row, col).x + 50.f, indexToPos(row, col).y + 26.f});
                    peas[row].push_back(aux);
                    grid[row][col].time = 0;
                }
            }
        }
    }
}

void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]){
    for (int row = 0; row < 5; row++){
        for (auto itP = peas[row].begin(); itP != peas[row].end();){
            bool peaHit = false;
            itP->sprite.move({elapsed.asSeconds()*300, 0});
            for(auto itZ = zombies[row].begin(); itZ != zombies[row].end();){
                if(itP->sprite.getPosition().x >= itZ->sprite.getPosition().x + 15.f){
                    peaHit = true;
                    itZ->damage(PEA_DMG);

                    if(itZ->health <= 0){
                        itZ = zombies[row].erase(itZ);
                        
                        if (zombiesLeft == 0 && allZombiesDead(zombies)) {
                            if (lvl == 1 && !levelUp) {
                                levelUp = true;
                                levelUpTimer = 0.0f;
                            }
                            else if (lvl == 2 && !gameWon) {
                                gameWon = true;
                                winTimer = 0.0f;
                            }
                        }
                        break;
                    } else {
                        ++itZ;
                    }
                    break; // pea can only hit one zombie
                } else {
                    ++itZ;
                }
            }
            if(peaHit){
                itP = peas[row].erase(itP);
            } else {
                if(itP->sprite.getPosition().x > 1300.f){
                    itP = peas[row].erase(itP);
                } else {
                    ++itP;
                }
            }
        }
    }
}
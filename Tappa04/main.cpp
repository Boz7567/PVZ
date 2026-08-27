#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

const float baseStartX = 490.0f;
const float baseStartY = 82.0f;
const float baseTileWidth = 108.0f;
const float baseTileHeight = 114.0f;
const float scaleX = 1280.0f / 1532.0f;
const float scaleY = 720.0f / 688.0f;

enum Grid{
    EMPTY, PEA, SUN
};

enum ZombieType{
    BASIC, CONE, BUCKET
};

struct Zombie{
    sf::Sprite sprite;
    int row;
    // int health = 100;
    // Add a custom constructor to initialize the sprite right away
    // ZombieType type; // do this later
    Zombie(const sf::Texture& texture, int row) : sprite(texture), row(row) {}
};

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa04 - Placing Plants");

Grid **grid;
sf::Texture Ptexture;
sf::Texture Stexture;
sf::Texture zombieTexture;
sf::Texture backgroundTexture;
sf::Texture peaIconTexture;
sf::Texture sunIconTexture;

sf::Vector2f indexToPos(int i, int j){
    float posX = (baseStartX + j * baseTileWidth) * scaleX;
    float posY = (baseStartY + i * baseTileHeight) * scaleY;

    return {posX, posY};
}

void drawGameBoard(){
    for(int i = 0; i<5; i++){
        for(int j = 0; j<9; j++){
            if(grid[i][j] == PEA){
                sf::Sprite peashooter(Ptexture);
                sf::Vector2f temp = {0,0};
                temp = indexToPos(i, j);
                peashooter.setPosition(temp);
                window.draw(peashooter);
            }
            if(grid[i][j] == SUN){
                sf::Sprite sunflower(Stexture);
                sf::Vector2f temp = {0,0};
                temp = indexToPos(i, j);
                sunflower.setPosition(temp);
                window.draw(sunflower);
            }
        }
    }
}


int main()
{
    srand(time(NULL));
    if (!Ptexture.loadFromFile("../resources/peashooter.png")){
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/peashooter.png" << std::endl;
        return -1;
    }
    if (!Stexture.loadFromFile("../resources/sunflower.png")){
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/sunflower.png" << std::endl;
        return -1;
    }
    if (!zombieTexture.loadFromFile("../resources/zombie.png")){
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/zombie1.png" << std::endl;
        return -1;
    }
    if (!backgroundTexture.loadFromFile("../resources/background.png"))
    {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/background.png" << std::endl;
        return -1;
    }
    if (!peaIconTexture.loadFromFile("../resources/peaicon.png"))
    {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/peaicon.png" << std::endl;
        return -1;
    }
    if (!sunIconTexture.loadFromFile("../resources/sunicon.png"))
    {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/sunicon.png" << std::endl;
        return -1;
    }
    sf::Sprite peaIcon(peaIconTexture);
    peaIcon.setPosition({10.0f, 20.0f});
    sf::Sprite sunIcon(sunIconTexture);
    sunIcon.setPosition({10.0f, 80.0f});

    grid = new Grid*[5];
    for(int i = 0; i < 5; i++){
        grid[i] = new Grid[9];
        for(int j = 0; j<9; j++){
            grid[i][j] = EMPTY;
        }
    }

    

    window.setFramerateLimit(60);

    sf::Sprite background(backgroundTexture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    background.setScale({scaleX, scaleY});

    Grid selection = EMPTY;
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if(event->is<sf::Event::MouseButtonPressed>()){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if(mousePos.x >= peaIcon.getPosition().x && mousePos.x <= peaIcon.getPosition().x + peaIconTexture.getSize().x * scaleX &&
                   mousePos.y >= peaIcon.getPosition().y && mousePos.y <= peaIcon.getPosition().y + peaIconTexture.getSize().y * scaleY){
                    selection = PEA;
                }
                else if(mousePos.x >= sunIcon.getPosition().x && mousePos.x <= sunIcon.getPosition().x + sunIconTexture.getSize().x * scaleX &&
                        mousePos.y >= sunIcon.getPosition().y && mousePos.y <= sunIcon.getPosition().y + sunIconTexture.getSize().y * scaleY){
                    selection = SUN;
                }
                else{
                    for(int i = 0; i<5; i++){
                        for(int j = 0; j<9; j++){
                            sf::Vector2f tilePos = indexToPos(i, j);
                            if(mousePos.x >= tilePos.x && mousePos.x <= tilePos.x + baseTileWidth * scaleX &&
                            mousePos.y >= tilePos.y && mousePos.y <= tilePos.y + baseTileHeight * scaleY){
                                if(grid[i][j] == EMPTY){
                                    grid[i][j] = selection;
                                }
                            }
                        }
                    }
                    selection = EMPTY;  // reset selection after having placed plant
                }
            }
        }

        window.draw(background);
        window.draw(peaIcon);
        window.draw(sunIcon);
        if(selection == 1){     // highlight selection
            sf::FloatRect bounds = peaIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }
        if(selection == 2){
            sf::FloatRect bounds = sunIcon.getGlobalBounds();
            sf::RectangleShape overlay(bounds.size);
            overlay.setPosition(bounds.position);
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);
        }
        drawGameBoard();
        window.display();
    }

    return 0;
}
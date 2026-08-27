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
    // ZombieType type; // do this later
    // Add a custom constructor to initialize the sprite right away
    Zombie(const sf::Texture& texture, int row) : sprite(texture), row(row) {}
};

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa03 - Zombies");

Grid **grid;
sf::Texture Ptexture;
sf::Texture Stexture;
sf::Texture zombieTexture;

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
    grid = new Grid*[5];
    for(int i = 0; i < 5; i++){
        grid[i] = new Grid[9];
        for(int j = 0; j<9; j++){
            grid[i][j] = EMPTY;
        }
    }

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

    if (!zombieTexture.loadFromFile("../resources/zombie.png")){
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/zombie1.png" << std::endl;
        return -1;
    }

    Zombie* zombies = new Zombie[5]{
        Zombie(zombieTexture, rand()%5),
        Zombie(zombieTexture, rand()%5),
        Zombie(zombieTexture, rand()%5),
        Zombie(zombieTexture, rand()%5),
        Zombie(zombieTexture, rand()%5)
    };

    for(int i = 4; i>=0; i--){
        sf::Vector2f pos = indexToPos(zombies[i].row, 8); // Start at the rightmost column
        zombies[i].sprite.setPosition({pos.x + 300, pos.y - 55});
    }


    grid[0][0] = SUN;
    grid[1][0] = SUN;
    grid[2][0] = SUN;
    grid[3][0] = SUN;
    grid[4][0] = SUN;
    
    //classic game board

    grid[0][1] = PEA;
    grid[1][1] = PEA;
    grid[2][1] = PEA;
    grid[3][1] = PEA;
    grid[4][1] = PEA;

    // 4. Main loop
    while (window.isOpen())
    {
        // Process events (SFML 3 uses std::optional)
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear (not strictly needed because we draw over everything, but good practice)
        window.clear();
        // Draw the background
        window.draw(background);
        drawGameBoard();
        for(int i = 4; i>=0; i--){
            window.draw(zombies[i].sprite);
            zombies[i].sprite.move({-1.0f, 0.0f}); // Move the zombie left by 1 pixel
        }// Display
        window.display();
    }

    return 0;
}
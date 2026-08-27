#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const float baseStartX = 440.1f;
const float baseStartY = 91.5f;
const float baseTileWidth = 97.0f;
const float baseTileHeight = 127.3f;

enum Grid {EMPTY, PEA, SUN};

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Tappa02 - Board and plants");
Grid **grid;
sf::Texture Ptexture;
sf::Texture Stexture;
sf::Vector2u g_textureSize;

sf::Vector2f indexToPos(int i, int j) {


    // Compute scaling from actual texture and window sizes
    sf::Vector2u winSize = window.getSize();
    float scaleX = static_cast<float>(winSize.x) / g_textureSize.x;
    float scaleY = static_cast<float>(winSize.y) / g_textureSize.y;

    float posX = (baseStartX + j * baseTileWidth) * scaleX;
    float posY = (baseStartY + i * baseTileHeight) * scaleY;
    return {posX, posY};
}

void drawGameBoard() {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 9; ++j) {
            sf::Vector2f pos = indexToPos(i, j);
            if (grid[i][j] == PEA) {
                sf::Sprite peashooter(Ptexture);
                peashooter.setPosition(pos);
                window.draw(peashooter);
            } else if (grid[i][j] == SUN) {
                sf::Sprite sunflower(Stexture);
                sunflower.setPosition(pos);
                window.draw(sunflower);
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    if (!Ptexture.loadFromFile("../resources/peashooter.png")) {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/peashooter.png" << std::endl;
        return -1;
    }
    if (!Stexture.loadFromFile("../resources/sunflower.png")) {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/sunflower.png" << std::endl;
        return -1;
    }

    // Allocate grid
    grid = new Grid*[5];
    for (int i = 0; i < 5; ++i) {
        grid[i] = new Grid[9];
        for (int j = 0; j < 9; ++j)
            // grid[i][j] = EMPTY;
            grid[i][j] = static_cast<Grid>(rand()%3);   //generating random board
    }

    window.setFramerateLimit(60);

    // Load background
    sf::Texture texture;
    if (!texture.loadFromFile("../resources/background.png")) {
        std::cerr << "ERRORE: impossibile caricare l'immagine ../resources/background.png" << std::endl;
        return -1;
    }
    g_textureSize = texture.getSize();

    sf::Sprite background(texture);
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u texSize = texture.getSize();
    background.setScale({
        static_cast<float>(winSize.x) / texSize.x,
        static_cast<float>(winSize.y) / texSize.y
    });

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(background);
        drawGameBoard();
        window.display();
    }

    // Cleanup (optional)
    for (int i = 0; i < 5; ++i){
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
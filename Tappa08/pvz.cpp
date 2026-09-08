#include "pvz.h"

sf::Texture Ptexture;
sf::Texture Stexture;
sf::Texture backgroundTexture;
sf::Texture peaIconTexture;
sf::Texture sunIconTexture;
sf::Texture sunBoxTexture;
sf::Texture sunTexture;
sf::Texture peaTexture;
sf::Texture lvlBoxTexture;
sf::Texture zombieTexture;
sf::Texture zombieConeTexture;
Plant **grid;

float g_scaleX = 1.0f;      // will be overwritten
float g_scaleY = 1.0f;      // will be overwritten
sf::Vector2u g_textureSize;

sf::Vector2f indexToPos(int i, int j) {
    float posX = (baseStartX + j * baseTileWidth) * g_scaleX;
    float posY = (baseStartY + i * baseTileHeight) * g_scaleY;
    return {posX, posY};
}

void drawGameBoard(sf::RenderWindow &window) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j].getType() == PEA) {
                sf::Sprite peashooter(Ptexture);
                peashooter.setPosition(indexToPos(i, j));
                window.draw(peashooter);
            }
            else if (grid[i][j].getType() == SUN) {
                sf::Sprite sunflower(Stexture);
                sunflower.setPosition(indexToPos(i, j));
                window.draw(sunflower);
            }
        }
    }
}

bool loadTextures() {
    if (!Ptexture.loadFromFile("../resources/peashooter.png")) {
        std::cerr << "ERRORE: peashooter.png\n";
        return false;
    }
    if (!Stexture.loadFromFile("../resources/sunflower.png")) {
        std::cerr << "ERRORE: sunflower.png\n";
        return false;
    }
    // if (!zombieTexture.loadFromFile("../resources/zombie.png")) {
    //     std::cerr << "ERRORE: zombie.png\n";
    //     return false;
    // }
    if (!backgroundTexture.loadFromFile("../resources/background.png")) {
        std::cerr << "ERRORE: background.png\n";
        return false;
    }
    if (!peaIconTexture.loadFromFile("../resources/peaicon.png")) {
        std::cerr << "ERRORE: peaicon.png\n";
        return false;
    }
    if (!sunIconTexture.loadFromFile("../resources/sunicon.png")) {
        std::cerr << "ERRORE: sunicon.png\n";
        return false;
    }
    if (!sunBoxTexture.loadFromFile("../resources/sun-box.png")) {
        std::cerr << "ERRORE: sun-box.png\n";
        return false;
    }
    if (!sunTexture.loadFromFile("../resources/sun.png")) {
        std::cerr << "ERRORE: sun.png\n";
        return false;
    }
    if (!peaTexture.loadFromFile("../resources/pea.png")) {
        std::cerr << "ERRORE: pea.png\n";
        return false;
    }
    if (!lvlBoxTexture.loadFromFile("../resources/lvl-box.png")) {
        std::cerr << "ERRORE: lvl-box.png\n";
        return false;
    }    
    if (!zombieTexture.loadFromFile("../resources/zombies.png")) {
        std::cerr << "ERRORE: zombies.png\n";
        return false;
    }    
    if (!zombieConeTexture.loadFromFile("../resources/zombies-cone.png")) {
        std::cerr << "ERRORE: zombies-cone.png\n";
        return false;
    }
    return true;
}
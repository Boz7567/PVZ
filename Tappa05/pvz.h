#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#pragma once

const float baseStartX = 420.0f;
const float baseStartY = 95.0f;
const float baseTileWidth = 100.0f;
const float baseTileHeight = 128.0f;

enum PlantType { EMPTY, SUN, PEA };
enum ZombieType { BASIC, CONE, BUCKET };

class Plant {
private:
    PlantType type;
public:
    Plant() : type(EMPTY) {}
    Plant(PlantType t) : type(t) {}
    PlantType getType() const { return type; }
};

class Zombie {
public:
    int row;
    Zombie(int r) : row(r) {}
};

extern sf::Texture Ptexture;
extern sf::Texture Stexture;
extern sf::Texture zombieTexture;
extern sf::Texture backgroundTexture;
extern sf::Texture peaIconTexture;
extern sf::Texture sunIconTexture;

extern Plant **grid;

extern float g_scaleX;
extern float g_scaleY;
extern sf::Vector2u g_textureSize;

sf::Vector2f indexToPos(int i, int j);
void drawGameBoard(sf::RenderWindow &window);
bool loadTextures();
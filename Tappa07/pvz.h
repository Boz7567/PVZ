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

class Plant{
private:
    PlantType type;
    int health = 100;
public:
    sf::Clock clock;    // serves as cooldown (for spawning suns if sunflower, for shooting if peashooter)
    Plant() : type(EMPTY) {}
    Plant(PlantType t) : type(t) {}
    PlantType getType() const { return type; }
    void damage(int i){
        health -=i;
    }
    bool isDead(){
        return this->health<=0;
    }
};

class Zombie{
public:
    int row;
    int health = 100;
    sf::Sprite sprite;
    Zombie(sf::Texture& t) : sprite(t) {
        row = rand()%5;
    }
};

class Sun{
public:
    sf::Clock clock;
    sf::Sprite sprite;
    bool falling = true;
    float stop_coord = 600.f;

    // Constructor that takes a texture reference
    Sun(const sf::Texture& texture) : sprite(texture) {
        // Set origin to the center so rotation/positioning is easier
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        stop_coord -= rand()%300;
        clock.start();
    }
};

class Pea{
public:
    sf::Sprite sprite;
    int row;

    Pea(sf::Texture& texture, int row) : sprite(texture){
        this->row = row;
    }
};

extern sf::Texture Ptexture;
extern sf::Texture Stexture;
extern sf::Texture zombieTexture;
extern sf::Texture backgroundTexture;
extern sf::Texture peaIconTexture;
extern sf::Texture sunIconTexture;
extern sf::Texture sunBoxTexture;
extern sf::Texture sunTexture;
extern sf::Texture peaTexture;
extern sf::Font sunValueFont;

extern Plant **grid;

extern float g_scaleX;
extern float g_scaleY;
extern sf::Vector2u g_textureSize;

sf::Vector2f indexToPos(int i, int j);
void drawGameBoard(sf::RenderWindow &window);
bool loadTextures();
void sunHandler(std::vector<sf::Sprite> &suns, sf::Clock &skySunSpawner);
void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void zombieHandler(std::vector<Zombie> zombies[5], sf::Clock &zombieSpawner);
// void sunFlowerHandler();
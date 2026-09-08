#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "animation.h"

#pragma once

extern sf::Texture Ptexture;
extern sf::Texture Stexture;
extern sf::Texture backgroundTexture;
extern sf::Texture peaIconTexture;
extern sf::Texture sunIconTexture;
extern sf::Texture sunBoxTexture;
extern sf::Texture sunTexture;
extern sf::Texture peaTexture;
extern sf::Texture lvlBoxTexture;
extern sf::Texture zombieTexture;
extern sf::Texture zombieConeTexture;

const float baseStartX = 420.0f;
const float baseStartY = 95.0f;
const float baseTileWidth = 100.0f;
const float baseTileHeight = 128.0f;

const int ZOMBIE_FRAME_WIDTH  = 543;
const int ZOMBIE_FRAME_HEIGHT = 814;

extern float g_scaleX;
extern float g_scaleY;
extern sf::Vector2u g_textureSize;
const int ZOMBIE_SHEET_COLS = 5;
const int ZOMBIE_SHEET_ROWS = 6;
const float ZOMBIE_SWITCH_TIME = 0.25f; //adjust


enum PlantType { EMPTY, SUN, PEA };
enum ZombieType { BASIC, CONE, BUCKET };

class Plant{
private:
    PlantType type;
    int health = 100;
public:
    float time = 8; //seconds, same as before, but using global clock (if it's just getting placed it's immediately ready to shoot/produce suns)
    int cost;
    Plant() : type(EMPTY) {}
    Plant(PlantType t) : type(t) {
        switch(t){
            case PEA:
                cost = 100;
                break;  
            case SUN:
                cost = 50;
                break;
            default:
                cost = 0;
        }
    }
    PlantType getType(){
        return type;
    }
    void damage(int i){
        if(i <=0){
            std::cerr<<"ERROR: damage can't be negative";
        }
        health -=i;
    }
    bool isDead(){
        return this->health<=0;
    }
};

class Zombie {
public:
    int row;
    int health = 200;
    ZombieType type;
    sf::Sprite sprite;
    Animation walkAnim;
    float attackCooldown = 0.0f;

    // Constructor: texture is passed by reference, we pass its address to Animation
    Zombie(sf::Texture& texture, int cols, int rows, float switchTime, int row, ZombieType type)
        : sprite(texture), walkAnim(&texture, cols, rows, ZOMBIE_FRAME_WIDTH, ZOMBIE_FRAME_HEIGHT,  switchTime), row(row), type(type) {


        if (type == CONE){
            health = 570;
        }
        // else if (type == BUCKET) health = 1300;
        else health = 200; // BASIC

        walkAnim.applyToSprite(sprite);

        // Scale to fit tile height (uses global g_scaleY)
        float desiredHeight = 1.8 * baseTileHeight * g_scaleY;
        float actualHeight = walkAnim.getFrameHeight();
        float scale = desiredHeight / actualHeight;
        sprite.setScale({scale, scale});

        sprite.setOrigin({0, sprite.getLocalBounds().size.y});
    }

    void damage(int d){
        if(d<=0){
            return;
        }
        health-=d;
        if(health <= 200 && this->type != BASIC){
            this->sprite.setTexture(zombieTexture);
            walkAnim.setTexture(&zombieTexture); // update the internal pointer
            walkAnim.applyToSprite(sprite);
            this->type = BASIC;
        }
    }

    void update(float deltaTime) {
        walkAnim.update(deltaTime);
        walkAnim.applyToSprite(sprite);
    }
};

class Sun{
public:
    float time = 0;
    sf::Sprite sprite;
    bool falling = true;
    float stop_coord = 600.f;

    // Constructor that takes a texture reference
    Sun(const sf::Texture& texture) : sprite(texture) {
        // Set origin to the center so rotation/positioning is easier
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        stop_coord -= rand()%300;
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

// extern sf::Texture coneTexture;

extern sf::Font sunValueFont;

extern Plant **grid;

sf::Vector2f indexToPos(int i, int j);
void drawGameBoard(sf::RenderWindow &window);
bool loadTextures();
void sunHandler(std::vector<Sun> &suns);
void sunSpawner(std::vector<Sun> &suns);
void peaHandler(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void peaSpawner(std::vector<Pea> peas[5], std::vector<Zombie> zombies[5]);
void zombieHandler(std::vector<Zombie> zombies[5]);
void zombieSpawner(std::vector<Zombie> zombies[5]);
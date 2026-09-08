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
extern sf::Texture nutIconTexture;
extern sf::Texture sunBoxTexture;
extern sf::Texture sunTexture;
extern sf::Texture peaTexture;
extern sf::Texture lvlBoxTexture;
extern sf::Texture zombieBoxTexture;
extern sf::Texture zombieTexture;
extern sf::Texture zombieConeTexture;
extern sf::Texture zombieEatingTexture;
extern sf::Texture zombieEatingConeTexture;
extern sf::Texture zombieHeadTexture;


const float baseStartX = 430.0f;
const float baseStartY = 95.0f;
const float baseTileWidth = 100.0f;
const float baseTileHeight = 128.0f;

const int LVL1 = 1;
const int LVL2 = 4;

const int ZOMBIE_FRAME_WIDTH  = 543;
const int ZOMBIE_FRAME_HEIGHT = 814;
const int ZOMBIE_SPEED = 10;

const int ZOMBIE_FRAME_WIDTH_E = 181;
const int ZOMBIE_FRAME_HEIGHT_E = 271;  //adjust

extern float g_scaleX;
extern float g_scaleY;
extern sf::Vector2u g_textureSize;
const int ZOMBIE_SHEET_COLS = 5;
const int ZOMBIE_SHEET_ROWS = 6;
const float ZOMBIE_SWITCH_TIME = 0.2f; //adjust

const int PEA_DMG = 25;

enum PlantType { EMPTY, SUN, PEA, NUT };
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
            case NUT:
                cost = 50;
                health = 1000;
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
    Animation eatAnim;
    bool eating = false;
    float attackCooldown = 0.0f;

    float walkScale;
    float eatScale;

    Zombie(sf::Texture& texture, sf::Texture& eatTexture, int cols, int rows,
        float switchTime, int row, ZombieType type)
        : sprite(texture),
        walkAnim(&texture, cols, rows, ZOMBIE_FRAME_WIDTH, ZOMBIE_FRAME_HEIGHT, switchTime),
        eatAnim(&eatTexture, cols, rows, ZOMBIE_FRAME_WIDTH_E, ZOMBIE_FRAME_HEIGHT_E, switchTime/3),
        row(row), type(type) {

        if (type == CONE) {
            health = 570;
        } else {
            health = 200;
        }

        float desiredHeight = 1.8f * baseTileHeight * g_scaleY;


        walkScale = desiredHeight / walkAnim.getFrameHeight();
        eatScale  = desiredHeight / eatAnim.getFrameHeight();// * 1.5; //adjust this

        sprite.setScale({walkScale, walkScale});

        walkAnim.applyToSprite(sprite);

        sprite.setOrigin({0, sprite.getLocalBounds().size.y});
    }

    void damage(int d){
        if(d<=0){
            return;
        }
        health-=d;
        if(health <= 200 && this->type != BASIC){
            this->sprite.setTexture(zombieTexture);
            walkAnim.setTexture(&zombieTexture); 
            eatAnim.setTexture(&zombieEatingTexture);
            walkAnim.applyToSprite(sprite);
            this->type = BASIC;
        }
    }

    void update(float deltaTime) {
        if (eating) {
            sprite.setScale({eatScale, eatScale});
            eatAnim.update(deltaTime);
            eatAnim.applyToSprite(sprite);
            sprite.setOrigin({0, static_cast<float>(eatAnim.getFrameHeight())});
        } else {
            sprite.setScale({walkScale, walkScale});
            walkAnim.update(deltaTime);
            walkAnim.applyToSprite(sprite);
            sprite.setOrigin({0, static_cast<float>(walkAnim.getFrameHeight())});
        }
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
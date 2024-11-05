#include <iostream>
#include <utility>

#include "weaponsConfig.h"

class Player; // TODO include actual player class

class Bullet // Base class for Bullets
{
    // TODO add getters and setters
    // TODO add logic so the bullet doesn't travel too many blocks. If it doesn't hit a player or a wall,I think it still shouldn't travel across the whole map
protected:
    float speed;
    int damage;
    std::pair<float, float> position;
    std::pair<float, float> direction;

public:
    Bullet(float spd, int dmg, std::pair<float, float> pos, std::pair<float, float> dir)
        : speed(spd), damage(dmg), position(pos), direction(dir) {}

    virtual void move()
    {
        position.first += speed * direction.first;
        position.second += speed * direction.second;
        std::cout << "Bullet moved to position: (" << position.first << ", " << position.second << ")" << std::endl;
    }

    virtual void collide(Player& player)
    {
        //player->health -= damage;  //TO DO include player class and subtract damage from health
        //std::cout << "Bullet collided with player! Damage dealt: " << damage << ". Player health: " << player->health << std::endl;
    }

    virtual ~Bullet(){};

    // checks if the current bullet collided with a player
    bool checkCollision(const Player& player)
    {
        //TODO IMPLEMENT CHECK COLLISION LOGIC
    }
};
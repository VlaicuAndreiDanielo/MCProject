#include <iostream>
#include <vector>
#include <utility>
#include <math.h>

#include "weaponsConfig.h"

class Player; // TODO include actual player class
class Bullet; // TO DO make bullet class a header and include it

// Base class for Weapons
class Weapon
{
protected:
    std::vector<Bullet*> bullets; // all bullets shot by the weapon

public:
    virtual void shoot() = 0;

    void updateBullets(std::vector<Player>& players)
    {
        for (auto bullet : bullets)
        {
            //bullet->move();  // TO DO add bullet header and uncomment
            for (auto& player : players)
            {
                //if (bullet->checkCollision(player))  // TO DO add bullet header and uncomment
                    //bullet->collide(player);  // TO DO add bullet header and uncomment
            }
        }
    }

    virtual ~Weapon()
    {
        for (auto bullet : bullets)
            delete bullet;
    }
};

// Derived class for weapons with limited ammo
class LimitedAmmoWeapon : public Weapon
{
protected:
    int ammoCount;
    virtual Bullet* createBullet() = 0; // will be implemented in each type of weapon so that it creates its own type of bullet

public:
    LimitedAmmoWeapon(int initialAmmo) : ammoCount(initialAmmo) {}

    void shoot() override
    {
        if (ammoCount > 0)
        {
            bullets.push_back(createBullet()); // Create a specific type of bullet
            ammoCount--;
            std::cout << "Shooting a bullet! Shots left: " << ammoCount << std::endl;
        }
        else
        {
            std::cout << "Out of ammo!" << std::endl;
            /*
            TODO delete the weapon from the player once it has 0 bullets left
            (unless we decide to add ammo crates, but it would cluster the menu by having too many guns
            if the player opens multiple boxes, so I think we should let the player have only one type of special weapon
            If he opens a box and he already has a special weapon, either add bullets to the current one or give the
            new type of weapon drawn from the box)
            */
        }
    }
};

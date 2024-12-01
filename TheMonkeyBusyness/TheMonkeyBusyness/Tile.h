#pragma once
#include "TileType.h"
#include <string>
#include "GameObject.h"
#include <crow.h>

class Tile : public GameObject
{
private:
    TileType m_tileType;
    int m_hp; // Health for destructible walls
    bool m_playerOccupied;
    bool m_isPassable;  // Can units walk through this tile?
    bool m_isShootable; // Can bullets or projectiles pass through this tile?

public:
    Tile(TileType type);
    ~Tile();
    void setType(TileType type);
    TileType getType() const;

    void setOccupied(bool occ);
    bool getOccupied() const;

    // Methods for managing HP
    void setHP(int health);
    int getHP() const;
    void takeDamage(int damage); // Decrease HP

    crow::json::wvalue toJson() const;
private:
    void updateProperties(); // Automatically adjust properties based on tile type
};


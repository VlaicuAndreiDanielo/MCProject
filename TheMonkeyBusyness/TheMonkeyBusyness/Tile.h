#pragma once
#include "TileType.h"
#include <string>
class Tile
{
private:
    TileType m_tileType;
    int m_hp; // Health for destructible walls
    bool m_playerOccupied;

public:
    Tile(TileType type);

    void setType(TileType type);
    TileType getType() const;

    void setOccupied(bool occ);
    bool getOccupied() const;

    // Methods for managing HP
    void setHP(int health);
    int getHP() const;
    void takeDamage(int damage); // Decrease HP
};


#include "Tile.h"
#include <iostream>

Tile::Tile(TileType type)
    : m_tileType{ type }, m_hp{ 0 }, m_playerOccupied{ false }, m_isPassable{ true }, m_isShootable{ true } {
    if (type == TileType::DestructibleWall) {
        m_hp = 3; // Default HP for destructible walls
    }
    updateProperties();
}

Tile::~Tile() {

}

void Tile::setType(TileType type) {
    m_tileType = type;
    if (type == TileType::DestructibleWall) {
        m_hp = 3; // Reset HP for destructible walls
    }
    else {
        m_hp = 0; // Other tiles have no HP
    }
    updateProperties(); // Adjust passable and shootable properties
}

TileType Tile::getType() const {
    return m_tileType;
}

void Tile::setOccupied(bool occ) {
    this->m_playerOccupied = occ;
}

bool Tile::getOccupied() const {
    return m_playerOccupied;
}

void Tile::setHP(int health) {
    m_hp = health;
}

int Tile::getHP() const {
    return m_hp;
}

void Tile::takeDamage(int damage) {
    if (m_tileType == TileType::DestructibleWall || m_tileType == TileType::FakeDestructibleWall) {
        m_hp -= damage;
        if (m_hp <= 0) {
            if (m_tileType == TileType::FakeDestructibleWall) {
                // Trigger explosion (logic to be implemented elsewhere)
                std::cout << "Fake wall exploded!\n";
            }
            m_tileType = TileType::Empty; // Turn into empty tile
            updateProperties(); // Update properties after type change
        }
    }
}

void Tile::updateProperties() {
    switch (m_tileType) {
    case TileType::Empty:
    case TileType::Grass:
        m_isPassable = true;
        m_isShootable = true;
        break;
    case TileType::DestructibleWall:
        m_isPassable = false;
        m_isShootable = false;
        break;
    case TileType::FakeDestructibleWall:
        m_isPassable = false;
        m_isShootable = false;
        break;
    case TileType::IndestructibleWall:
        m_isPassable = false;
        m_isShootable = false;
        break;
    case TileType::Water:
        m_isPassable = false;
        m_isShootable = true;
        break;
    case TileType::Lava:
        m_isPassable = false;
        m_isShootable = true;
        break;
    default:
        m_isPassable = true;
        m_isShootable = true;
    }
}

#include "Tile.h"
#include <iostream>

Tile::Tile(TileType type) : m_tileType{ type }, m_hp{ 0 }, m_playerOccupied{ false } {
	if (type == TileType::DestructibleWall) {
		m_hp = 3; // Default HP for destructible walls
	}
}
Tile::~Tile()
{

}
void Tile::setType(TileType type) {
	m_tileType = type;
	if (type == TileType::DestructibleWall) {
		m_hp = 3; // Reset HP for destructible walls
	}
	else {
		m_hp = 0; // Other tiles have no HP
	}
}

TileType Tile::getType() const 
{ 
	return m_tileType; 
}

void Tile::setOccupied(bool occ)
{
	this->m_playerOccupied = occ;
}

bool Tile::getOccupied() const
{
	return m_playerOccupied;
}

// Methods for managing HP
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
		}
	}
}

crow::json::wvalue Tile::toJson() const {
	crow::json::wvalue tileJson;
	tileJson["type"] = static_cast<int>(m_tileType);
	tileJson["hp"] = m_hp;
	tileJson["occupied"] = m_playerOccupied;
	return tileJson;
}


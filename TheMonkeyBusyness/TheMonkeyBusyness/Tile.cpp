#include "Tile.h"
#include <iostream>

Tile::Tile(TileType type) :m_tileType{ type }, playerOccupied {false}
{
}

void Tile::setType(TileType type)
{
	this->m_tileType = type;
}

TileType Tile::getType() const 
{ 
	return m_tileType; 
}

void Tile::setOcccupied(bool occ)
{
	this->playerOccupied = occ;
}

bool Tile::getOccupied()
{
	return playerOccupied;
}

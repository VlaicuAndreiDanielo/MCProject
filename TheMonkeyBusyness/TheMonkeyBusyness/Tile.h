#pragma once
#include "TileType.h"
#include <string>
class Tile
{
private:
	TileType m_tileType;
	bool playerOccupied;
public:
	Tile(TileType type);
	void setType(TileType type);
	TileType getType() const;
	void setOcccupied(bool occ);
	bool getOccupied();
};


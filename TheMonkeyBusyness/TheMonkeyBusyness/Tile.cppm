export module Tile;

import <string>;
import TileType;

export class Tile
{
private:
    TileType m_tileType;
    bool playerOccupied;
public:
    Tile(TileType type);
    void setType(TileType type);
    TileType getType() const;
    void setOccupied(bool occ);
    bool getOccupied() const;
};
#pragma once
#include <vector>
#include <ctime>
#include <algorithm>
import Tile;
import TileType;

class Arena
{
private:
    int m_dim;
    std::vector<std::vector<Tile>> m_mapa;
    int m_numSpawns;
public:

    Arena(int dim, int numSpawns);

    std::vector<std::vector<Tile>> generate_map(int dim, int numSpawns);

    void print_map() const;

    void generateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim);
};

#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Tile.h"

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
};
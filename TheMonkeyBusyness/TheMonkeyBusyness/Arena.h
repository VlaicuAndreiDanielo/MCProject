#pragma once
#include <vector>
#include <ctime>
#include <algorithm>
#include "Tile.h"
#include "TileType.h"
#include <QtGui/qpainter.h>
#include <cstdint>
#include "ConstantValues.h"
class Arena
{
private:
    int m_dim;
    std::vector<std::vector<Tile>> m_map;
    int m_numSpawns;
    //temporary color squares for the tiles;
    std::vector<QColor> squareColors{ Qt::green, Qt::black, Qt::red, Qt::yellow, Qt::blue, Qt::darkGreen, Qt::darkRed, Qt::magenta }; 
public:

    Arena(int dim=100, int numSpawn=1);

    std::vector<std::vector<Tile>> generate_map(int dim, int numSpawns);

    void print_map() const;

    void generateBigLiquid(std::vector<std::vector<Tile>>& map, int dim);
    void generateSmallLiquid(std::vector<std::vector<Tile>>& map, int dim);
    TileType getRandomLiquid();
    void draw(QPainter& painter)const;
};

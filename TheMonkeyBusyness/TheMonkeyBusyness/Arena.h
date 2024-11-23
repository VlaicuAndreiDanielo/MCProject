#pragma once
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include<random>
#include "Tile.h"
#include "TileType.h"
#include <QtGui/qpainter.h>
#include <cstdint>
#include "ConstantValues.h"

class Arena
{
private:
    int m_dim;
    std::vector<std::vector<Tile>> m_mapa;
    int m_numSpawns;
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
        }
    };
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash> m_teleporterConnections;
    //temporary color squares for the tiles;
    std::vector<QColor> squareColors{ Qt::green, Qt::black, Qt::red, Qt::yellow, Qt::blue, Qt::darkGreen, Qt::darkRed, Qt::magenta };
public:

    Arena(int dim = 100, int numSpawn = 1);

    std::vector<std::vector<Tile>> generate_map(int dim, int numSpawns);

    void print_map() const;


    void generateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim);
    void generateSmallLiquid(std::vector<std::vector<Tile>>& mapa, int dim);
    TileType getRandomLiquid();
    std::pair<int, int> getConnectedTeleporter(int x, int y) const;
    void generateInitialSpawns(std::vector<std::vector<Tile>>& mapa, int dim, int numSpawns);
    void generateGrass(std::vector<std::vector<Tile>>& mapa);
    void pairTeleporters(const std::vector<std::pair<int, int>>& teleporters);
    void placeTeleporters(std::vector<std::vector<Tile>>& mapa);
    std::pair<int, int> generateTeleporterPosition(const std::vector<std::vector<Tile>>& mapa, int border, int offset, std::mt19937& gen);
    void draw(QPainter& painter)const;
};

#pragma once
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <random>
#include "Tile.h"
#include "TileType.h"
#include <cstdint>
#include "ConstantValues.h"

class Arena
{
private:
    int m_dim;
    std::vector<std::vector<Tile>> m_mapa;
    int m_numSpawns;
    std::vector<std::pair<int, int>> m_spawnPositions;
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
        }
    };
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash> m_teleporterConnections;
public:

    Arena(int dim = 100, int numSpawn = 1);

    //no need for a singleton now

    ////Forcing Singleton pattern, that means that only 1 arena can ever exist during runtime
    //Arena(const Arena&) = delete;
    //Arena& operator=(const Arena&) = delete;

    //// Static function to get the single instance of Arena
    //static Arena& Instance() {
    //    static Arena instance; // The single instance
    //    return instance;
    //}



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
    void applyCellularAutomata(std::vector<std::vector<Tile>>& mapa, int dim, int iterations, TileType type);
    //void draw(QPainter& painter)const;

    Tile& GetTile(int line, int col);
    std::pair<int, int> GetSpawn();
    void triggerExplosion(int x, int y);

    crow::json::wvalue ToJson() const;
};

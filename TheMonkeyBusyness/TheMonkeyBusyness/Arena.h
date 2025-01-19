#pragma once
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <random>
#include "Tile.h"
#include "TileType.h"
#include <string>
#include <crow.h>
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

    Arena(int dim = 50, int numSpawn = 10);

    std::vector<std::vector<Tile>> GenerateMap(int dim, int numSpawns);

    void PrintMap() const;

    void GenerateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim);
    void GenerateSmallLiquid(std::vector<std::vector<Tile>>& mapa, int dim);
    TileType GetRandomLiquid();
    std::pair<int, int> GetConnectedTeleporter(int x, int y) const;
    void GenerateInitialSpawns(std::vector<std::vector<Tile>>& mapa, int dim, int numSpawns);
    void GenerateGrass(std::vector<std::vector<Tile>>& mapa);
    void PairTeleporters(const std::vector<std::pair<int, int>>& teleporters);
    void PlaceTeleporters(std::vector<std::vector<Tile>>& mapa);
    std::pair<int, int> GenerateTeleporterPosition(const std::vector<std::vector<Tile>>& mapa, int border, int offset, std::mt19937& gen);
    void ApplyCellularAutomata(std::vector<std::vector<Tile>>& mapa, int dim, int iterations, TileType type);
    void GenerateDestructibleWalls(std::vector<std::vector<Tile>>& mapa, int probability);
    void TransformDestructibleWalls(std::vector<std::vector<Tile>>& mapa, int dim, int indestructibleProbability, int fakeProbability);
    void GenerateLake(std::vector<std::vector<Tile>>& mapa, int dim, TileType type);
    void GenerateRiver(std::vector<std::vector<Tile>>& mapa, int dim, TileType type);
    void PlaceOppositeEdgeTeleporters(std::vector<std::vector<Tile>>& mapa, int dim, std::pair<int, int> start, std::pair<int, int> end);

    Tile& GetTile(int line, int col);
    std::pair<int, int> GetSpawn();
    void TriggerExplosion(int x, int y);

    crow::json::wvalue ToJson() const;
};

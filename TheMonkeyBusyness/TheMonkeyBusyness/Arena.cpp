#include "Arena.h"
#include "TileType.h"
#include "Tile.h"
#include <random>
#include <queue>
#include <iostream>

Arena::Arena(int dim, int numSpawns) : m_dim{ dim }, m_numSpawns{ numSpawns }
{
    this->m_map = generate_map(dim, numSpawns);
}

std::vector<std::vector<Tile>> Arena::generate_map(int dim, int numSpawns)
{
    // Initializare harta cu spatii goale
    std::vector<std::vector<Tile>> map(dim, std::vector<Tile>(dim, Tile(TileType::Empty)));

    for (int i = 0; i < dim; i++) {
        map[i][0].setType(TileType::IndestructibleWall);
        map[i][dim - 1].setType(TileType::IndestructibleWall);
    }

    for (int j = 0; j < dim; j++) {
        map[0][j].setType(TileType::IndestructibleWall);
        map[dim - 1][j].setType(TileType::IndestructibleWall);
    }

    generateBigLiquid(map, dim);
    generateSmallLiquid(map, dim);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    for (int y = 1; y < dim - 1; ++y) {
        for (int x = 1; x < dim - 1; ++x) {
            if (distrib(gen) < 35 && map[y][x].getType() == TileType::Empty) { // 35% sanse pentru ziduri initiale
                map[y][x].setType(TileType::DestructibleWall);
            }
        }
    }

    // Iteratii de Cellular Automata
    int iterations = 2;
    for (int it = 0; it < iterations; ++it) {
        std::vector<std::vector<Tile>> newMap = map;

        for (int y = 1; y < dim - 1; ++y) {
            for (int x = 1; x < dim - 1; ++x) {
                if (map[y][x].getType() != TileType::Empty && map[y][x].getType() != TileType::DestructibleWall) {
                    continue;  // Skip further processing for this tile
                }

                int wallCount = 0;
                // Numara peretii vecini
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (map[y + dy][x + dx].getType() == TileType::DestructibleWall) {
                            wallCount++;
                        }
                    }
                }

                // Aplica regula Cellular Automata
                if (wallCount >= 4) {
                    newMap[y][x].setType(TileType::DestructibleWall); // mai multi pereti vecini -> devine obstacol
                }
                else {
                    newMap[y][x].setType(TileType::Empty); // mai putini pereti vecini -> devine spatiu liber
                }
            }
        }
        map = newMap;
    }

    //transforma peretii destructibili in indestructibili
    int indestructibleWallProbability = 20; // 20% sansa de a transforma in perete indestructibil
    for (int y = 1; y < dim - 1; ++y) {
        for (int x = 1; x < dim - 1; ++x) {
            if (map[y][x].getType() == TileType::DestructibleWall && distrib(gen) < indestructibleWallProbability) {
                map[y][x].setType(TileType::IndestructibleWall);
            }
        }
    }

    return map;
}



void Arena::generateBigLiquid(std::vector<std::vector<Tile>>& map, int dim)
{

}

void Arena::generateSmallLiquid(std::vector<std::vector<Tile>>& map, int dim) {
    TileType type = getRandomLiquid();

    // Determinam locatia spre margine
    int startX = (rand() % 2 == 0) ? (rand() % (dim / 4)) : (dim - 1 - rand() % (dim / 4));
    int startY = (rand() % 2 == 0) ? (rand() % (dim / 4)) : (dim - 1 - rand() % (dim / 4));

    int lakeRadius = rand() % (dim / 10) + 1; // raza mica pentru lac

    for (int dy = -lakeRadius; dy <= lakeRadius; ++dy) {
        for (int dx = -lakeRadius; dx <= lakeRadius; ++dx) {
            int x = startX + dx;
            int y = startY + dy;
            if (x >= 1 && x < dim - 1 && y >= 1 && y < dim - 1) {
                if (std::sqrt(dx * dx + dy * dy) <= lakeRadius) {
                    map[y][x].setType(type);
                }
            }
        }
    }
}

TileType Arena::getRandomLiquid()
{
    std::srand(std::time(0));
    return (rand() % 2 == 0) ? TileType::Water : TileType::Lava;
}

void Arena::draw(QPainter& painter) const
{
    painter.save();

    

    painter.restore();
}

// Afisarea hartii in consola
void Arena::print_map() const
{
    for (const auto& row : m_map) {
        for (const auto& tile : row) {
            switch (tile.getType()) {
            case TileType::Empty: std::cout << ' '; break;
            case TileType::IndestructibleWall: std::cout << '#'; break;
            case TileType::DestructibleWall: std::cout << 'D'; break;
            case TileType::Water: std::cout << '~'; break;
            case TileType::Grass: std::cout << 'G'; break;
            case TileType::Spawn: std::cout << 'S'; break;
            case TileType::Teleporter: std::cout << 'T'; break;
            case TileType::Lava: std::cout << 'L'; break;
            }
        }
        std::cout << '\n';
    }
}


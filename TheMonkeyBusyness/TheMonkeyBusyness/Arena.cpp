#include "Arena.h"
import TileType;
#include <random>
#include <queue>
#include <iostream>

Arena::Arena(int dim, int numSpawns) : m_dim{ dim }, m_numSpawns{ numSpawns }
{
    this->m_mapa = generate_map(dim, numSpawns);
}

std::vector<std::vector<Tile>> Arena::generate_map(int dim, int numSpawns)
{
    // Initializare harta cu spatii goale
    std::vector<std::vector<Tile>> mapa(dim, std::vector<Tile>(dim, Tile(TileType::Empty)));

    for (int i = 0; i < dim; i++) {
        mapa[i][0].setType(TileType::IndestructibleWall);
        mapa[i][dim - 1].setType(TileType::IndestructibleWall);
    }

    for (int j = 0; j < dim; j++) {
        mapa[0][j].setType(TileType::IndestructibleWall);
        mapa[dim - 1][j].setType(TileType::IndestructibleWall);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    for (int y = 1; y < dim - 1; ++y) {
        for (int x = 1; x < dim - 1; ++x) {
            if (distrib(gen) < 35 && mapa[y][x].getType()==TileType::Empty) { // 35% sanse pentru ziduri initiale
                mapa[y][x].setType(TileType::DestructibleWall);
            }
        }
    }

    // Iteratii de Cellular Automata
    int iterations = 2;
    for (int it = 0; it < iterations; ++it) {
        std::vector<std::vector<Tile>> newMap = mapa;

        for (int y = 1; y < dim - 1; ++y) {
            for (int x = 1; x < dim - 1; ++x) {
                if (mapa[y][x].getType() != TileType::Empty && mapa[y][x].getType() != TileType::DestructibleWall) {
                    continue;  // Skip further processing for this tile
                }
                
                int wallCount = 0;
                // Numara peretii vecini
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (mapa[y + dy][x + dx].getType() == TileType::DestructibleWall) {
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
        mapa = newMap;
    }

    //transforma peretii destructibili in indestructibili
    int indestructibleWallProbability = 20; // 20% sansa de a transforma in perete indestructibil
    for (int y = 1; y < dim - 1; ++y) {
        for (int x = 1; x < dim - 1; ++x) {
            if (mapa[y][x].getType() == TileType::DestructibleWall && distrib(gen) < indestructibleWallProbability) {
                mapa[y][x].setType(TileType::IndestructibleWall);
            }
        }
    }

    return mapa;
}

void Arena::generateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim) {
    TileType type = getRandomLiquid();

    int centerX = dim / 2;
    int centerY = dim / 2;

    // Rau sau lac mare
    bool isRiver = rand() % 2 == 0;

    if (isRiver) {
        // Start the river from one edge and span to the opposite edge
        int startX = (rand() % 2 == 0) ? 1 : dim - 2; // Start from left or right edge
        int startY = (rand() % 2 == 0) ? 1 : dim - 2; // Start from top or bottom edge
        int endX = dim - startX - 1;
        int endY = dim - startY - 1;

        int x = startX;
        int y = startY;

        int width = 3;  // River width
        bool gapAdded = false;  // Track if a gap has been added

        while ((x != endX || y != endY) && x > 0 && x < dim - 1 && y > 0 && y < dim - 1) {
            // Add the river tiles along the current position
            for (int dx = -width / 2; dx <= width / 2; ++dx) {
                for (int dy = -width / 2; dy <= width / 2; ++dy) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 1 && nx < dim - 1 && ny >= 1 && ny < dim - 1) {
                        // Create a gap if it's not yet added and a random condition is met
                        if (!gapAdded && rand() % 100 < 10) {
                            gapAdded = true;
                        }
                        else {
                            mapa[ny][nx].setType(type);
                        }
                    }
                }
            }

            // Randomly adjust the direction slightly to create curvature
            x += (rand() % 3) - 1;  // -1, 0, or 1
            y += (rand() % 3) - 1;  // -1, 0, or 1

            // Ensure we continue moving toward the target end edge
            if (x < endX) x++;
            if (x > endX) x--;
            if (y < endY) y++;
            if (y > endY) y--;
        }
    }
    else {
        // Generam un lac mare central
        int lakeRadius = dim / 6 + rand() % (dim / 8); // raza lacului

        for (int dy = -lakeRadius; dy <= lakeRadius; ++dy) {
            for (int dx = -lakeRadius; dx <= lakeRadius; ++dx) {
                int x = centerX + dx;
                int y = centerY + dy;
                if (x >= 1 && x < dim - 1 && y >= 1 && y < dim - 1) {
                    if (std::sqrt(dx * dx + dy * dy) <= lakeRadius) {
                        mapa[y][x].setType(type);
                    }
                }
            }
        }
    }
}

// Afisarea hartii in consola
void Arena::print_map() const
{
    for (const auto& row : m_mapa) {
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
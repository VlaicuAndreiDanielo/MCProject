#include "Arena.h"
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

    generateBigLiquid(mapa, dim);
    generateSmallLiquid(mapa, dim);

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

    generateInitialSpawns(mapa, numSpawns, 8);

    return mapa;
}

void Arena::generateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim) {
    TileType type = getRandomLiquid();

    int centerX = dim / 2;
    int centerY = dim / 2;

    // Rau sau lac mare
    bool isRiver = rand() % 2 == 0;

    if (isRiver) {
        // Randomly select starting edge and ending edge for the river
        int startX, startY, endX, endY;
        int edge = rand() % 4;
        switch (edge) {
        case 0:  // Start from top edge
            startX = rand() % (dim - 2) + 1;
            startY = 1;
            endX = startX;
            endY = dim - 2;
            break;
        case 1:  // Start from bottom edge
            startX = rand() % (dim - 2) + 1;
            startY = dim - 2;
            endX = startX;
            endY = 1;
            break;
        case 2:  // Start from left edge
            startX = 1;
            startY = rand() % (dim - 2) + 1;
            endX = dim - 2;
            endY = startY;
            break;
        case 3:  // Start from right edge
            startX = dim - 2;
            startY = rand() % (dim - 2) + 1;
            endX = 1;
            endY = startY;
            break;
        }

        int x = startX;
        int y = startY;
        int width = 3;  // Set river width
        // Determine the gap location as a fraction of the river length
        int riverLength = std::abs(endX - startX) + std::abs(endY - startY);
        int gapPosition = rand() % (riverLength / 2) + riverLength / 4;  // Position for the gap, roughly in the middle
        int stepCount = 0;

        // Initial direction toward end point
        int directionX = (endX > startX) ? 1 : (endX < startX) ? -1 : 0;
        int directionY = (endY > startY) ? 1 : (endY < startY) ? -1 : 0;

        while ((x != endX || y != endY) && x > 0 && x < dim - 1 && y > 0 && y < dim - 1) {
            // Check if the current position is the gap
            bool isGap = (stepCount == gapPosition);

            // Place river tiles in a wider area around the main path, except at the gap
            if (!isGap) {
                for (int dx = -width / 2; dx <= width / 2; ++dx) {
                    for (int dy = -width / 2; dy <= width / 2; ++dy) {
                        int nx = x + dx;
                        int ny = y + dy;

                        if (nx >= 1 && nx < dim - 1 && ny >= 1 && ny < dim - 1) {
                            mapa[ny][nx].setType(type);
                        }
                    }
                }
            }

            // Randomly adjust direction to create curvature
            if (rand() % 100 < 20) {  // 20% chance to change direction
                directionX += (rand() % 3) - 1;  // Adjust by -1, 0, or 1
                directionY += (rand() % 3) - 1;
            }

            // Normalize direction to keep river flowing in the general direction of the endpoint
            if (directionX > 1) directionX = 1;
            if (directionX < -1) directionX = -1;
            if (directionY > 1) directionY = 1;
            if (directionY < -1) directionY = -1;

            // Move to the next position along the river path
            x += directionX;
            y += directionY;

            // Increment step counter
            stepCount++;
        }
    }
    else {
        // Generate a large lake with deformations
        int baseRadius = dim / 6 + rand() % (dim / 8);  // Base radius for the lake

        for (int dy = -baseRadius; dy <= baseRadius; ++dy) {
            for (int dx = -baseRadius; dx <= baseRadius; ++dx) {
                // Calculate the deformed radius by adding a small random amount
                int deformation = (rand() % 5) - 1; // Random value between -2 and 2
                int adjustedRadius = baseRadius + deformation;

                int x = centerX + dx;
                int y = centerY + dy;

                // Check if within bounds and if the point is within the deformed radius
                if (x >= 1 && x < dim - 1 && y >= 1 && y < dim - 1) {
                    if (std::sqrt(dx * dx + dy * dy) <= adjustedRadius) {
                        mapa[y][x].setType(type);
                    }
                }
            }
        }
    }
}

void Arena::generateSmallLiquid(std::vector<std::vector<Tile>>& mapa, int dim) {
    TileType type = getRandomLiquid();
    if (type == TileType::Water)
        type = TileType::Lava;
    else
        type = TileType::Water;

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
                    mapa[y][x].setType(type);
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

// Helper function to calculate the Euclidean distance between two points
double calculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Function to place spawns with a significant distance between them
void Arena::generateInitialSpawns(std::vector<std::vector<Tile>>& mapa, int numSpawns, int minDistance) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, m_dim - 2); // Avoid borders

    std::vector<std::pair<int, int>> spawnPositions;

    for (int i = 0; i < numSpawns; ++i) {
        int x, y;
        bool validSpawn;

        do {
            x = distrib(gen);
            y = distrib(gen);

            validSpawn = true;

            // Check if the selected tile is empty
            if (mapa[y][x].getType() != TileType::Empty) {
                validSpawn = false;
                continue;
            }

            // Check if the spawn is sufficiently far from existing spawns
            for (const auto& [sx, sy] : spawnPositions) {
                if (calculateDistance(x, y, sx, sy) < minDistance) {
                    validSpawn = false;
                    break;
                }
            }

            // Check if the 3x3 area around the tile is clear
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < m_dim && ny >= 0 && ny < m_dim) {
                        if (mapa[ny][nx].getType() != TileType::Empty) {
                            validSpawn = false;
                            break;
                        }
                    }
                }
                if (!validSpawn) break;
            }

        } while (!validSpawn);

        // Place the spawn
        mapa[y][x].setType(TileType::Spawn);
        spawnPositions.emplace_back(x, y);
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
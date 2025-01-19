#include "Arena.h"
#include "FastNoiseLite.h"
#include <queue>
#include <iostream>
#include <functional>
import TileTypeModule;
Arena::Arena(int dim, int numSpawns) : m_dim{ dim }, m_numSpawns{ numSpawns }
{
    this->m_mapa = generate_map(dim, numSpawns);
}

std::vector<std::vector<Tile>> Arena::generate_map(int dim, int numSpawns) {
    // Initialize map with empty spaces
    std::vector<std::vector<Tile>> mapa(dim, std::vector<Tile>(dim, Tile(TileType::Empty)));

    // Create indestructible walls around the edges
    for (int i = 0; i < dim; i++) {
        mapa[i][0].setType(TileType::IndestructibleWall);
        mapa[i][dim - 1].setType(TileType::IndestructibleWall);
    }
    for (int j = 0; j < dim; j++) {
        mapa[0][j].setType(TileType::IndestructibleWall);
        mapa[dim - 1][j].setType(TileType::IndestructibleWall);
    }

    // Generate liquids
    generateBigLiquid(mapa, dim);
    generateSmallLiquid(mapa, dim);

    // Generate destructible walls
    generateDestructibleWalls(mapa, 35); // 35% chance for initial destructible walls

    // Transform destructible walls into other types
    transformDestructibleWalls(mapa, dim, 10, 20); // 10% chance for indestructible, 20% for fake destructible

    // Add grass
    generateGrass(mapa);

    // Generate spawns and teleporters
    generateInitialSpawns(mapa, numSpawns, 8);
    placeTeleporters(mapa);

    return mapa;
}

Tile& Arena::GetTile(int line, int col)
{
    return this->m_mapa[line][col];
}

std::pair<int, int> Arena::GetSpawn() {
    if (m_spawnPositions.empty()) {
        throw std::runtime_error("No spawn positions available!");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, m_spawnPositions.size() - 1);

    int randomIndex = distrib(gen);
    return m_spawnPositions[randomIndex];
}

void Arena::applyCellularAutomata(std::vector<std::vector<Tile>>& mapa, int dim, int iterations, TileType type) {
    for (int it = 0; it < iterations; ++it) {
        std::vector<std::vector<Tile>> newMap = mapa;

        for (int y = 1; y < dim - 1; ++y) {
            for (int x = 1; x < dim - 1; ++x) {
                // Skip tiles that are not the specified type or empty
                if (mapa[y][x].getType() != TileType::Empty && mapa[y][x].getType() != type) {
                    continue;
                }

                int count = 0;

                // Count neighboring tiles
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue; // Skip the center tile
                        if (mapa[y + dy][x + dx].getType() == type) {
                            count++;
                        }
                    }
                }

                // Apply Cellular Automata rules
                if (count >= 4) {
                    newMap[y][x].setType(type); // More neighbors -> becomes specified type
                }
                else {
                    newMap[y][x].setType(TileType::Empty); // Fewer neighbors -> becomes empty
                }
            }
        }

        mapa = newMap; // Update the map after each iteration
    }
}

void Arena::generateDestructibleWalls(std::vector<std::vector<Tile>>& mapa, int probability) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    for (int y = 1; y < mapa.size() - 1; ++y) {
        for (int x = 1; x < mapa[0].size() - 1; ++x) {
            if (distrib(gen) < probability && mapa[y][x].getType() == TileType::Empty) {
                mapa[y][x].setType(TileType::DestructibleWall);
            }
        }
    }
    applyCellularAutomata(mapa, mapa.size(), 3, TileType::DestructibleWall);
}

void Arena::transformDestructibleWalls(std::vector<std::vector<Tile>>& mapa, int dim, int indestructibleProbability, int fakeProbability) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    for (int y = 1; y < dim - 1; ++y) {
        for (int x = 1; x < dim - 1; ++x) {
            if (mapa[y][x].getType() == TileType::DestructibleWall) {
                int randomValue = distrib(gen);
                if (randomValue < indestructibleProbability) {
                    mapa[y][x].setType(TileType::IndestructibleWall);
                }
                else if (randomValue < indestructibleProbability + fakeProbability) {
                    mapa[y][x].setType(TileType::FakeDestructibleWall);
                }
            }
        }
    }
}

void Arena::generateBigLiquid(std::vector<std::vector<Tile>>& mapa, int dim) {
    TileType type = getRandomLiquid();
    bool isRiver = rand() % 2 == 0;

    if (isRiver) {
        generateRiver(mapa, dim, type);
    }
    else {
        generateLake(mapa, dim, type);
    }
}

void Arena::generateLake(std::vector<std::vector<Tile>>& mapa, int dim, TileType type) {
    FastNoiseLite noise;
    noise.SetSeed(static_cast<int>(time(0)));
    noise.SetFrequency(0.1f); // Controls the level of detail in the noise pattern

    TileType liquidType = (rand() % 2 == 0) ? TileType::Water : TileType::Lava;

    // Randomly choose the center point for the lake
    int centerX = dim / 5 + rand() % (dim / 2);
    int centerY = dim / 5 + rand() % (dim / 2);

    int maxRadius = dim / 7 + rand() % (dim / 8);

    // Generate the lake
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            // Calculate distance from the center
            float distance = std::sqrt(std::pow(x - centerX, 2) + std::pow(y - centerY, 2));

            // Add Perlin noise to the distance calculation
            float noiseValue = noise.GetNoise((float)x, (float)y);

            // Base condition for being part of the lake
            if (distance + noiseValue * maxRadius / 4.0f <= maxRadius) {
                // Ensure we don't overwrite existing important tiles
                if (mapa[y][x].getType() == TileType::Empty) {
                    mapa[y][x].setType(liquidType);
                }
            }

            // Add fragmentation if it's lava and near the edge of the lake
            if (liquidType == TileType::Lava) {
                float edgeDistance = maxRadius - distance;
                if (edgeDistance >= 0 && edgeDistance < 3.0f) { // Edge threshold for fragmentation
                    if (noise.GetNoise((float)x, (float)y) > 0.6f) { // Add jagged noise
                        if (mapa[y][x].getType() == TileType::Empty) {
                            mapa[y][x].setType(TileType::Lava); // Add lava fragments
                        }
                    }
                }
            }
        }
    }
}

void Arena::generateRiver(std::vector<std::vector<Tile>>& mapa, int dim, TileType type) {
    int startX, startY, endX, endY;
    int edge = rand() % 4;

    // Determine the start edge and set the opposite edge for the endpoint
    switch (edge) {
    case 0: // Top to Bottom
        startX = rand() % (dim - 2) + 1;
        startY = 1;
        endX = rand() % (dim - 2) + 1;
        endY = dim - 2;
        break;

    case 1: // Bottom to Top
        startX = rand() % (dim - 2) + 1;
        startY = dim - 2;
        endX = rand() % (dim - 2) + 1;
        endY = 1;
        break;

    case 2: // Left to Right
        startX = 1;
        startY = rand() % (dim - 2) + 1;
        endX = dim - 2;
        endY = rand() % (dim - 2) + 1;
        break;

    case 3: // Right to Left
        startX = dim - 2;
        startY = rand() % (dim - 2) + 1;
        endX = 1;
        endY = rand() % (dim - 2) + 1;
        break;
    }

    int x = startX, y = startY;
    int width = dim / 20; // River width

    int directionX = (endX > startX) ? 1 : (endX < startX) ? -1 : 0;
    int directionY = (endY > startY) ? 1 : (endY < startY) ? -1 : 0;

    // Generate the river path
    while ((x != endX || y != endY) && x > 0 && x < dim - 1 && y > 0 && y < dim - 1) {
        for (int dx = -width / 2; dx <= width / 2; ++dx) {
            for (int dy = -width / 2; dy <= width / 2; ++dy) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 1 && nx < dim - 1 && ny >= 1 && ny < dim - 1) {
                    mapa[ny][nx].setType(type);
                }
            }
        }

        if (rand() % 100 < 20) {
            directionX += (rand() % 3) - 1;
            directionY += (rand() % 3) - 1;
        }

        directionX = std::clamp(directionX, -1, 1);
        directionY = std::clamp(directionY, -1, 1);

        x += directionX;
        y += directionY;
    }

    // Place teleporters at opposite edges to ensure accessibility
    placeOppositeEdgeTeleporters(mapa, dim, { startX, startY }, { endX, endY });
}

void Arena::placeOppositeEdgeTeleporters(std::vector<std::vector<Tile>>& mapa, int dim, std::pair<int, int> start, std::pair<int, int> end) {
    // Place teleporter at the start edge
    if (mapa[start.second][start.first].getType() == TileType::Empty) {
        mapa[start.second][start.first].setType(TileType::Teleporter);
    }

    // Place teleporter at the end edge
    if (mapa[end.second][end.first].getType() == TileType::Empty) {
        mapa[end.second][end.first].setType(TileType::Teleporter);
    }

    // Pair the teleporters
    m_teleporterConnections[start] = end;
    m_teleporterConnections[end] = start;
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

    m_spawnPositions = spawnPositions;
}

void Arena::generateGrass(std::vector<std::vector<Tile>>& mapa) {
    // Initialize random grass tiles
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    for (int y = 1; y < mapa.size() - 1; ++y) {
        for (int x = 1; x < mapa[0].size() - 1; ++x) {
            if (distrib(gen) < 35 && mapa[y][x].getType() == TileType::Empty) { // 35% chance for initial grass
                mapa[y][x].setType(TileType::Grass);
            }
        }
    }

    // Use the generic cellular automata function for grass refinement
    applyCellularAutomata(mapa, mapa.size(), 3, TileType::Grass);
}

std::pair<int, int> Arena::getConnectedTeleporter(int x, int y) const {
    auto it = m_teleporterConnections.find({ x, y });
    if (it != m_teleporterConnections.end()) {
        return it->second;
    }
    return { -1, -1 }; // Return an invalid location if no connection exists
}

void Arena::placeTeleporters(std::vector<std::vector<Tile>>& mapa) {
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> countDistrib(0, 2); // 0, 2, or 4 teleporters (in pairs)
    std::uniform_int_distribution<> borderOffsetDistrib(1, 8); // Distance from the border (1 to 8)

    int numTeleporters = countDistrib(gen) * 2; // Ensure an even number (0, 2, or 4)
    if (numTeleporters == 0) return;

    std::vector<std::pair<int, int>> teleporters;

    // Borders: 0 = top, 1 = bottom, 2 = left, 3 = right
    std::vector<int> borders = { 0, 1, 2, 3 };
    std::shuffle(borders.begin(), borders.end(), gen); // Randomize borders

    for (int i = 0; i < numTeleporters / 2; ++i) {
        if (borders.size() < 2) break; // Ensure enough distinct borders

        // Pick two distinct borders
        int border1 = borders.back();
        borders.pop_back();
        int border2 = borders.back();
        borders.pop_back();

        // Place teleporter on the first border
        std::pair<int, int> teleporter1 = generateTeleporterPosition(mapa, border1, borderOffsetDistrib(gen), gen);
        teleporters.push_back(teleporter1);
        mapa[teleporter1.second][teleporter1.first].setType(TileType::Teleporter);

        // Place teleporter on the second border
        std::pair<int, int> teleporter2 = generateTeleporterPosition(mapa, border2, borderOffsetDistrib(gen), gen);
        teleporters.push_back(teleporter2);
        mapa[teleporter2.second][teleporter2.first].setType(TileType::Teleporter);
    }

    // Pair the teleporters
    pairTeleporters(teleporters);
}

std::pair<int, int> Arena::generateTeleporterPosition(const std::vector<std::vector<Tile>>& mapa, int border, int offset, std::mt19937& gen) {
    std::uniform_int_distribution<> dimDistrib(1, m_dim - 2); // Avoid corners
    int x = 0, y = 0;

    switch (border) {
    case 0: // Top border
        x = offset;
        y = 1;
        break;
    case 1: // Bottom border
        x = offset;
        y = m_dim - 2;
        break;
    case 2: // Left border
        x = 1;
        y = offset;
        break;
    case 3: // Right border
        x = m_dim - 2;
        y = offset;
        break;
    }

    // Ensure the chosen tile is empty
    while (mapa[y][x].getType() != TileType::Empty) {
        offset = dimDistrib(gen);
        switch (border) {
        case 0: y = 1; x = offset; break;      // Top border
        case 1: y = m_dim - 2; x = offset; break; // Bottom border
        case 2: x = 1; y = offset; break;      // Left border
        case 3: x = m_dim - 2; y = offset; break; // Right border
        }
    }

    return { x, y };
}

void Arena::pairTeleporters(const std::vector<std::pair<int, int>>& teleporters) {
    if (teleporters.size() < 2) return;

    // Create teleporter connections
    for (size_t i = 0; i < teleporters.size(); i += 2) {
        auto t1 = teleporters[i];
        auto t2 = teleporters[i + 1];
        m_teleporterConnections[t1] = t2;
        m_teleporterConnections[t2] = t1;
    }
}

void Arena::triggerExplosion(int x, int y) {
    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    for (const auto& [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;

        if (nx >= 0 && nx < m_dim && ny >= 0 && ny < m_dim) {
            Tile& neighbor = m_mapa[ny][nx];
            if (neighbor.getType() == TileType::DestructibleWall) {
                neighbor.takeDamage(3); // Fully destroy adjacent destructible walls
            }
        }
    }
}

crow::json::wvalue Arena::ToJson() const {
    crow::json::wvalue arenaJson = crow::json::wvalue::list();

    for (int i = 0; i < m_dim; i++) {  // Iterate through rows
        crow::json::wvalue jsonRow = crow::json::wvalue::list();
        for (int j = 0; j < m_dim; j++) {  // Iterate through columns
            jsonRow[j] = static_cast<int>(m_mapa[i][j].getType());
        }
        arenaJson[i] = std::move(jsonRow);
    }

    return arenaJson;
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
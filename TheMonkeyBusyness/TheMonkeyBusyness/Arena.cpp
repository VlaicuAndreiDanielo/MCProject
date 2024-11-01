#include "Arena.h"
#include <random>
#include <queue>

Arena::Arena(int dim, int numSpawns) : m_dim {dim}, m_numSpawns {numSpawns}
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

    srand(static_cast<unsigned>(time(0)));

    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            if (mapa[y][x].getType() == TileType::Empty && (rand() % 100 < 20)) { // Probabilitate de 20%
                TileType obstacleType = static_cast<TileType>(2 + rand() % 6); // Apa, iarba, zid destructibil, zid indestructibil, teleporter, lava
                mapa[y][x].setType(obstacleType);
            }
        }
    }

    // Spawn-urile
    std::vector<std::pair<int, int>> spawnPositions = { {1, 1}, {1, dim - 2}, {dim - 2, 1}, {dim - 2, dim - 2} };
    for (int i = 0; i < numSpawns && i < 4; ++i) {
        int x = spawnPositions[i].first;
        int y = spawnPositions[i].second;

        // Golim o zona de 3x3 in jurul spawn-ului
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = x + dx;
                int ny = y + dy;

                // Asiguram ca nu incercam sa accesam bordura
                if (nx > 0 && nx < dim - 1 && ny > 0 && ny < dim - 1) {
                    mapa[ny][nx].setType(TileType::Empty);
                }
            }
        }
        // Setam tile-ul de spawn
        mapa[y][x].setType(TileType::Spawn);
    }
    return mapa;
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
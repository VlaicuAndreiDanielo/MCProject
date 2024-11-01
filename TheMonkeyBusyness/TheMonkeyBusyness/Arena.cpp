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
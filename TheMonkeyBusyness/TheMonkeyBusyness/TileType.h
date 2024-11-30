#pragma once
#include <cstdint>
enum class TileType : uint8_t
{
	Empty,
	Spawn,
	IndestructibleWall,
	DestructibleWall,
	Water,
	Grass,
	Lava,
	Teleporter,
	FakeDestructibleWall
};
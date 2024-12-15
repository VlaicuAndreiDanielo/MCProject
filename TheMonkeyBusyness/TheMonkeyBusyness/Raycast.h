#pragma once
#include "Vector2.h"
#include "iostream"
#include "Arena.h"
#include "Tile.h"
#include "ConstantValues.h"

class Cast {
public:
    GameObject* Raycast(Vector2 origin, Vector2 direction, float maxDistance);
	Arena* m_arena;
};
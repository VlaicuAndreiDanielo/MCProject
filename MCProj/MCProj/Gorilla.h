#pragma once
#include "Character.h"

class Gorilla : public Character
{
public:
	Gorilla();
	void activateSpecialAbility() override;
	void monkeyEvolution() override;
	int getSheild() const { return shieldPoints; }
	~Gorilla() = default;
private:
	int shieldPoints = 0;
	bool shieldActive;
	std::chrono::steady_clock::time_point abilityStartTime;
};


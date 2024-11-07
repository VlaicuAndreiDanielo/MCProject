#pragma once
#include <chrono>
#include "Character.h"
#include<iostream>

class AnubisBaboon : public Character
{
public:
	AnubisBaboon();
	void activateSpecialAbility() override;
	void monkeyEvolution() override;
	void update();
	~AnubisBaboon() = default;
private:
	bool bonusSpeedActive;
	std::chrono::steady_clock::time_point abilityStartTime;
};


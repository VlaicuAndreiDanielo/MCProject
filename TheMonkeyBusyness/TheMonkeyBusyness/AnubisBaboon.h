#pragma once
#include <chrono>
#include "Character.h"
#include<iostream>

class AnubisBaboon : public Character
{
public:
	AnubisBaboon();
	void ActivateSpecialAbility() override;
	void MonkeyEvolution() override;
	void Update();
	~AnubisBaboon() = default;
private:
	bool m_bonusSpeedActive;
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


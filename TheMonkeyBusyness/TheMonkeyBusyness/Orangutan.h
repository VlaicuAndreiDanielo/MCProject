#pragma once
#include <random>
#include "Character.h"

class Orangutan : public Character
{
public:
	Orangutan();
	void ActivateSpecialAbility()override;
	void MonkeyEvolution() override;
	~Orangutan() = default;
private:
	int GetRandomHealthRegen(int min, int max) const;
	std::chrono::steady_clock::time_point m_lastAbilityUse;
};


#pragma once
#include <random>
//#include "Character.h"
#include <chrono>
#include <iostream>
import CharacterModule;
class Orangutan : public Character
{
public:
	Orangutan();
	void ActivateSpecialAbility()override;
	~Orangutan() = default;
private:
	int GetRandomHealthRegen(int min, int max) const;
	std::chrono::steady_clock::time_point m_lastAbilityUse;
};


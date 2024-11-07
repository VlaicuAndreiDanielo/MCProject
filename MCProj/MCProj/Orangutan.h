#pragma once
#include <random>
#include "Character.h"

class Orangutan : public Character
{
public:
	Orangutan();
	void activateSpecialAbility()override;
	void monkeyEvolution() override;
	~Orangutan() = default;
private:
	int getRandomHealthRegen(int min, int max) const;
};


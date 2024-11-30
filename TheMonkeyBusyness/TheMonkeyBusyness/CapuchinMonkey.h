#pragma once
#include "Character.h"


class CapuchinMonkey : public Character
{
public:
	CapuchinMonkey();
	void activateSpecialAbility() override;
	void update();
	void monkeyEvolution() override;
	~CapuchinMonkey() = default;
private:
	bool isAgilityActive = false;
	int agilityDuration = 15;
	int agilityBoostAmount = 5;
	int baseSpeed = 8;
};


#pragma once
#include "Character.h"

class HowlerMonkey : public Character
{
public:
	HowlerMonkey();
	void activateSpecialAbility()override;
	void update();
	void takeDamage(int damage);
	void monkeyEvolution() override;
	~HowlerMonkey() = default;
private:
	bool isInvulnerable = false;  
	int soundBarrierDuration = 5;  
	int baseHP = 600;
};


#pragma once
#include <iostream>
class Character
{
public:
	Character();
	Character(int hp, int speed,float cooldown , float remainingtime)
		: HP(hp), speed(speed), cooldownTime(cooldown), remainingCooldown(remainingtime) {};
	virtual void attack() const;

	virtual void activateSpecialAbility() = 0;
	virtual void monkeyEvolution() = 0; 

	virtual ~Character() = default;
	
protected:
	int HP;
	int speed;
	float cooldownTime;
	float remainingCooldown;
	int evolutionLevel = 0;
};


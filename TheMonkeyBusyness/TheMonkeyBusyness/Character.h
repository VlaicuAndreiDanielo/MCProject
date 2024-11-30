#pragma once
#include <iostream>
#include <chrono>
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

	int getHealth() const { return HP; }
	int getSpeed() const { return speed; }
	float getCooldownTime() const { return cooldownTime; }
	
protected:
	int HP;
	int speed;
	float cooldownTime;
	float remainingCooldown;
	int evolutionLevel = 0;
};


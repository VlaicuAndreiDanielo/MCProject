#pragma once
#include <iostream>
class Character
{
public:
	Character();
	Character(int hp, int speed,float cooldown , float remainingtime)
		: HP(hp), speed(speed), cooldownTime(cooldown), remainingTime(remainingtime) {};
	virtual void attack() const;

	virtual void activateSpecialAbility() const = 0;
	virtual void monkeyEvolution() = 0; 

	virtual ~Character() = default;
	
protected:
	int HP;
	int speed;
	float cooldownTime;
	float remainingTime;
};


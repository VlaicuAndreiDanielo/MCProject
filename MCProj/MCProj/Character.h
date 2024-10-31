#pragma once
class Character
{
public:
	Character();
	Character(int hp, int speed,float cooldown , float remainingtime);
	virtual void attack() const;

	virtual void activateSpecialAbility() const = 0;
	virtual void monkeyEvolution() = 0; 

	virtual ~Character() = default;
	
private:
	int HP;
	int speed;
	float cooldownTime;
	float remainingTime;
};


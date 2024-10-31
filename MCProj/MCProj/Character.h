#pragma once
class Character
{
public:
	Character();
	Character(int hp, int speed);
	virtual void attack() const;

	virtual void activateSpecialAbility() const = 0;
	virtual void monkeyEvolution() const = 0; 

	virtual ~Character() = default;
	
private:
	int HP;
	int speed;
	float coolDown;
	float remainingTime;
};


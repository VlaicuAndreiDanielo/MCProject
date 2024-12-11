#pragma once
#include <iostream>
#include <chrono>
class Character
{
public:
	Character();
	Character(int hp, int speed,float cooldown , float remainingtime)
		: m_HP(hp), m_speed(speed), m_cooldownTime(cooldown), m_remainingCooldown(remainingtime) {};
	virtual void Attack() const;

	virtual void ActivateSpecialAbility() = 0;
	virtual void MonkeyEvolution() = 0; 

	virtual ~Character() = default;

	int GetHealth() const { return m_HP; }
	int GetSpeed() const { return m_speed; }
	float GetCooldownTime() const { return m_cooldownTime; }
	
protected:
	int m_HP;
	int m_speed;
	float m_cooldownTime;
	float m_remainingCooldown;
	int EvolutionLevel = 0;
};


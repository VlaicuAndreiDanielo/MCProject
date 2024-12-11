#pragma once
#include "Character.h"

class HowlerMonkey : public Character
{
public:
	HowlerMonkey();
	void ActivateSpecialAbility()override;
	void Update();
	void TakeDamage(int damage);
	void MonkeyEvolution() override;
	~HowlerMonkey() = default;
private:
	bool m_isInvulnerable = false;  
	int m_soundBarrierDuration = 5;  
	int m_baseHP = 600;
};


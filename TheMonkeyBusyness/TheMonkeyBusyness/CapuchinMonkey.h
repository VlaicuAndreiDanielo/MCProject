#pragma once
#include "Character.h"


class CapuchinMonkey : public Character
{
public:
	CapuchinMonkey();
	void ActivateSpecialAbility() override;
	void Update();
	void MonkeyEvolution() override;
	~CapuchinMonkey() = default;
private:
	bool m_isAgilityActive = false;
	int m_agilityDuration = 15;
	int m_agilityBoostAmount = 5;
	int m_baseSpeed = 8;
};


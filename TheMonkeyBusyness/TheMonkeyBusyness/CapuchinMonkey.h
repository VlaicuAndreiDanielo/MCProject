#pragma once
#include "Character.h"


class CapuchinMonkey : public Character
{
public:
	CapuchinMonkey();
	void ActivateSpecialAbility() override;
	~CapuchinMonkey() = default;
private:
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


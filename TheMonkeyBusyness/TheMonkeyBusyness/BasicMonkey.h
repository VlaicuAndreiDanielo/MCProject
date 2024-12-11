#pragma once
#include "Character.h"
#include <chrono>


class BasicMonkey : public Character
{
public:
	BasicMonkey();
	void ActivateSpecialAbility() override;
	void MonkeyEvolution() override;
	void Update();
	~BasicMonkey() = default;
private:
	bool m_speedBoostActive;
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


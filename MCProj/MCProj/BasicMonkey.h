#pragma once
#include "Character.h"
#include <chrono>


class BasicMonkey : public Character
{
public:
	BasicMonkey();
	void activateSpecialAbility() override;
	void monkeyEvolution() override;
	void update();
	~BasicMonkey() = default;
private:
	bool speedBoostActive;
	std::chrono::steady_clock::time_point abilityStartTime;
};


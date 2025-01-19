#pragma once
//#include "Character.h"
#include <chrono>
#include <iostream>
import CharacterModule;

class BasicMonkey : public Character
{
public:
	BasicMonkey();
	void ActivateSpecialAbility() override;
	~BasicMonkey() = default;
private:
	bool m_speedBoostActive;
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


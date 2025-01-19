#pragma once
//#include "Character.h"
#include <chrono>
#include <iostream>
import CharacterModule;

class CapuchinMonkey : public Character
{
public:
	CapuchinMonkey();
	void ActivateSpecialAbility() override;
	~CapuchinMonkey() = default;
private:
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


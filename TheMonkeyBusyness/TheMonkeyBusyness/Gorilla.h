#pragma once
//#include "Character.h"
#include <chrono>
#include <iostream>
import CharacterModule;
class Gorilla : public Character
{
public:
	Gorilla();
	void ActivateSpecialAbility() override;
	~Gorilla() = default;
private:
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


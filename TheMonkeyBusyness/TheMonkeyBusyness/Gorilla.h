#pragma once
#include "Character.h"

class Gorilla : public Character
{
public:
	Gorilla();
	void ActivateSpecialAbility() override;
	~Gorilla() = default;
private:
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


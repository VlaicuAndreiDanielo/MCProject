#pragma once
#include "Character.h"

class Gorilla : public Character
{
public:
	Gorilla();
	void ActivateSpecialAbility() override;
	void MonkeyEvolution() override;
	int GetSheild() const { return m_shieldPoints; }
	~Gorilla() = default;
private:
	int m_shieldPoints = 0;
	bool m_shieldActive;
	std::chrono::steady_clock::time_point m_abilityStartTime;
};


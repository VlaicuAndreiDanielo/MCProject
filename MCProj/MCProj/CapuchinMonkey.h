#pragma once
#include "Character.h"

class CapuchinMonkey : public Character
{
public:
	CapuchinMonkey();
	void activateSpecialAbility() const override;
	void monkeyEvolution() override;
	~CapuchinMonkey() = default;
private:

};


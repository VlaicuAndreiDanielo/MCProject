#pragma once
#include "Character.h"


class BasicMonkey : public Character
{
public:
	BasicMonkey();
	void activateSpecialAbility() const override;
	void monkeyEvolution() override;
	~BasicMonkey() = default;
private:

};


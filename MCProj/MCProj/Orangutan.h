#pragma once
#include "Character.h"

class Orangutan : public Character
{
public:
	Orangutan();
	void activateSpecialAbility()const override;
	void monkeyEvolution() override;
	~Orangutan() = default;
private:

};


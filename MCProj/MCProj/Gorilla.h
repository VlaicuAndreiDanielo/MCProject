#pragma once
#include "Character.h"

class Gorilla : public Character
{
public:
	Gorilla();
	void activateSpecialAbility() override;
	void monkeyEvolution() override;
	~Gorilla() = default;
private:

};


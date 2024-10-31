#pragma once
#include "Character.h"

class Gorilla : public Character
{
public:
	Gorilla();
	void activateSpecialAbility() const override;
	void monkeyEvolution() override;
	~Gorilla() = default;
private:

};


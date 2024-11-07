#include "HowlerMonkey.h"

HowlerMonkey::HowlerMonkey()
	: Character(600, 6, 30, 0)
{}

void HowlerMonkey::activateSpecialAbility() const
{
	//TODO: implement function
}

void HowlerMonkey::monkeyEvolution()
{
	std::cout << "HowlerMonkey is evolving...\n";
	HP += 60;     // Crestem viata
	speed += 2;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

#include "CapuchinMonkey.h"

CapuchinMonkey::CapuchinMonkey()
	: Character(400, 8, 15, 0)
{}

void CapuchinMonkey::activateSpecialAbility() const
{
	//TODO: implement function
}

void CapuchinMonkey::monkeyEvolution()
{
	std::cout << "CapuchinMonkey is evolving...\n";
	HP += 75;     // Crestem viata
	speed += 3;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

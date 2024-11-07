#include "BasicMonkey.h"

BasicMonkey::BasicMonkey() 
	: Character(500, 10, 12, 0)
{}

void BasicMonkey::activateSpecialAbility() const
{
	//TODO: implement function
}

void BasicMonkey::monkeyEvolution()
{
	std::cout << "BasicMonkey is evolving...\n";
	HP += 75;     // Crestem viata
	speed += 3;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

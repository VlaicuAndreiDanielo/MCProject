#include "Gorilla.h"

Gorilla::Gorilla()
	: Character(1500, 4, 20, 0)
{}

void Gorilla::activateSpecialAbility() const
{
	//TODO: implement function
}

void Gorilla::monkeyEvolution()
{
	std::cout << "Gorilla is evolving...\n";
	HP += 75;     // Crestem viata
	speed += 3;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

#include "Orangutan.h"

Orangutan::Orangutan() 
	: Character(1200, 3, 20, 0)
{}

void Orangutan::activateSpecialAbility() const
{
	//TODO: implement function
}

void Orangutan::monkeyEvolution()
{
	std::cout << "Orangutan is evolving...\n";
	HP += 50;     // Crestem viata
	speed += 1;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

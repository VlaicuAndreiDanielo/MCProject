#include "AnubisBaboon.h"

AnubisBaboon::AnubisBaboon()
	: Character(800, 6, 30, 0)
{}

void AnubisBaboon::activateSpecialAbility() const
{
	//TODO: implement function
}

void AnubisBaboon::monkeyEvolution()
{
	std::cout << "AnubisBaboon is evolving...\n";
	HP += 50;     // Crestem viata
	speed += 2;   // Crestem viteza
	std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
}

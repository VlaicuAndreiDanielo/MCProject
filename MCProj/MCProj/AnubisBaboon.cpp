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
	if(evolutionLevel<5){
		std::cout << "AnubisBaboon is evolving...\n";
		HP += 50;     // Crestem viata
		speed += 2;   // Crestem viteza
		evolutionLevel++;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
    }
	else {
		std::cout << "AnubisBaboon has reached the maximum evolution level (5)!\n";
	}
}

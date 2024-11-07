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
	if(evolutionLevel < 5){
		std::cout << "Gorilla is evolving...\n";
		HP += 30;     // Crestem viata
		speed += 1;   // Crestem viteza
		evolutionLevel++;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
	}
	else {
		std::cout << "BasicMonkey has reached the maximum evolution level (5)!\n";
	}
}

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
	if(evolutionLevel<5){
		std::cout << "Orangutan is evolving...\n";
		HP += 50;     // Crestem viata
		speed += 2;   // Crestem viteza
		evolutionLevel++;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
	}
	else {
		std::cout << "Orangutan has reached the maximum evolution level (5)!\n";
	}
}

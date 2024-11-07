#include "CapuchinMonkey.h"

CapuchinMonkey::CapuchinMonkey()
	: Character(400, 8, 15, 0)
{}

void CapuchinMonkey::activateSpecialAbility() 
{
	//TODO: implement function
}

void CapuchinMonkey::monkeyEvolution()
{
	if(evolutionLevel < 5){
		std::cout << "CapuchinMonkey is evolving...\n";
		HP += 100;     // Crestem viata
		speed += 3;   // Crestem viteza
		evolutionLevel++;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
	}
	else {
		std::cout << "CapuchinMonkey has reached the maximum evolution level (5)!\n";
	}
}

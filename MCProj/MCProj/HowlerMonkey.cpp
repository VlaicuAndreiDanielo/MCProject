#include "HowlerMonkey.h"

HowlerMonkey::HowlerMonkey()
	: Character(600, 6, 30, 0)
{}

void HowlerMonkey::activateSpecialAbility()
{
	
}

void HowlerMonkey::monkeyEvolution()
{
	if (evolutionLevel < 5)
	{
		std::cout << "HowlerMonkey is evolving...\n";
		HP += 60;     // Crestem viata
		speed += 2;   // Crestem viteza
		evolutionLevel++ ;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
	}else {
		std::cout << "HowlerMonkey has reached the maximum evolution level (5)!\n";
	}
}

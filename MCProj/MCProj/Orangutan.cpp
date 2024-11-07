#include "Orangutan.h"

Orangutan::Orangutan() 
	: Character(1200, 3, 20, 0)
{}

void Orangutan::activateSpecialAbility() 
{   //Special Ability = Regereration;
	if (remainingCooldown <= 0) {
		std::cout << "Orangutan activates Regeneration!\n";
		// Randomizare pentru regenerare
		int regenerationAmount = getRandomHealthRegen(2, 5);
		HP += regenerationAmount;
		std::cout << "Orangutan regenerates " << regenerationAmount
			<< " HP. New HP: " << HP << "\n";

		remainingCooldown = cooldownTime; // Setează cooldown-ul la 60 secunde
	}
	else {
		std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
	}
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

int Orangutan::getRandomHealthRegen(int min, int max) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

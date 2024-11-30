#include "Gorilla.h"

Gorilla::Gorilla()
	: Character(1500, 4, 20, 0)
{}

void Gorilla::activateSpecialAbility() 
{   //Protection
    if (remainingCooldown <= 0) {
        std::cout << "Gorilla activates Shield!\n";

        // Activăm scutul de 50 de puncte
        shieldPoints = 50;
        shieldActive = true;
        abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "Gorilla gains a shield of 50 points!\n";
        std::cout << "Current Shield: " << shieldPoints << "\n";

        // Setăm cooldown-ul abilității la 20 de secunde
        remainingCooldown = cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
    }
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
		std::cout << "Gorilla has reached the maximum evolution level (5)!\n";
	}
}

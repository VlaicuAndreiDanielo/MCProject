#include "BasicMonkey.h"

BasicMonkey::BasicMonkey() 
	: Character(500, 10, 12, 0)
{}

void BasicMonkey::activateSpecialAbility() 
{ //Special Ability = Quick Escape 
    if (remainingCooldown <= 0) {
        std::cout << "BasicMonkey activates Speed Boost!\n";

        // Temporar boost la viteză pentru 25 de secunde
        speed += 10;
        speedBoostActive = true;
        abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "BasicMonkey gains +10 speed for 25 seconds!\n";
        std::cout << "New Speed: " << speed << "\n";

        // Setăm cooldown-ul abilității
        remainingCooldown = cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
    }
}

void BasicMonkey::monkeyEvolution()
{
	if(evolutionLevel < 5){
		std::cout << "BasicMonkey is evolving...\n";
		HP += 75;     // Crestem viata
		speed += 3;   // Crestem viteza
		evolutionLevel++;
		std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
		std::cout << "Evolution Level: " << evolutionLevel << "\n";
	}
	else {
		std::cout << "BasicMonkey has reached the maximum evolution level (5)!\n";
	}
}

void BasicMonkey::update()
{
    if (speedBoostActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - abilityStartTime).count();

        if (elapsedTime >= 25) {
            // Restaurăm viteza inițială după 25 de secunde
            speed -= 10;
            speedBoostActive = false;
            std::cout << "Speed Boost effect ended. Speed restored to " << speed << "\n";
        }
    }

    // Logica pentru cooldown
    if (remainingCooldown > 0) {
        remainingCooldown -= 1; // Presupunem că `update` este apelată la fiecare secundă
    }
}

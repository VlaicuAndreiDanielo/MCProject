#include "AnubisBaboon.h"

AnubisBaboon::AnubisBaboon()
	: Character(900, 6, 30, 0)
{}

void AnubisBaboon::activateSpecialAbility() 
{   // Special Ability = Rage
    if (remainingCooldown <= 0) {
        std::cout << "Anubis Baboon activates Rage!\n";

        // Permanent boost la HP
        HP += 500;

        // Temporar boost la viteză pentru 30 de secunde
        speed += 5;
        bonusSpeedActive = true;
        abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "Anubis Baboon gains 500 HP and temporary +5 speed for 30 seconds!\n";
        std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";

        // Setăm cooldown-ul abilității
        remainingCooldown = cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
    }
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

void AnubisBaboon::update()
{
    if (bonusSpeedActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - abilityStartTime).count();

        if (elapsedTime >= 30) {
            // Restaurăm viteza inițială după 30 de secunde
            speed -= 5;
            bonusSpeedActive = false;
            std::cout << "Rage effect ended. Speed restored to " << speed << "\n";
        }
    }

    // Logica pentru cooldown, dacă este necesară
    if (remainingCooldown > 0) {
        remainingCooldown -= 1; // Presupunem că `update` este apelată la fiecare secundă
    }
}

#include "Gorilla.h"

Gorilla::Gorilla()
	: Character(110, 135, 20, 0)
{}

void Gorilla::ActivateSpecialAbility() 
{   //Protection
    if (m_remainingCooldown <= 0) {
        std::cout << "Gorilla activates Shield!\n";

        // Activăm scutul de 50 de puncte
        m_shieldPoints = 50;
        m_shieldActive = true;
        m_abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "Gorilla gains a shield of 50 points!\n";
        std::cout << "Current Shield: " << m_shieldPoints << "\n";

        // Setăm cooldown-ul abilității la 20 de secunde
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}

void Gorilla::MonkeyEvolution()
{
	if(EvolutionLevel < 5){
		std::cout << "Gorilla is evolving...\n";
		m_HP += 30;     // Crestem viata
		m_speed += 1;   // Crestem viteza
		EvolutionLevel++;
		std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
		std::cout << "Evolution Level: " << EvolutionLevel << "\n";
	}
	else {
		std::cout << "Gorilla has reached the maximum evolution level (5)!\n";
	}
}

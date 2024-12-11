#include "AnubisBaboon.h"

AnubisBaboon::AnubisBaboon()
	: Character(900, 6, 30, 0)
{}

void AnubisBaboon::ActivateSpecialAbility() 
{   // Special Ability = Rage
    if (m_remainingCooldown <= 0) {
        std::cout << "Anubis Baboon activates Rage!\n";

        // Permanent boost la HP
        m_HP += 500;

        // Temporar boost la viteză pentru 30 de secunde
        m_speed += 5;
        m_bonusSpeedActive = true;
        m_abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "Anubis Baboon gains 500 HP and temporary +5 speed for 30 seconds!\n";
        std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";

        // Setăm cooldown-ul abilității
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}
void AnubisBaboon::MonkeyEvolution()
{
	if(EvolutionLevel<5){
		std::cout << "AnubisBaboon is evolving...\n";
		m_HP += 50;     // Crestem viata
		m_speed += 2;   // Crestem viteza
		EvolutionLevel++;
		std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
		std::cout << "Evolution Level: " << EvolutionLevel << "\n";
    }
	else {
		std::cout << "AnubisBaboon has reached the maximum evolution level (5)!\n";
	}
}

void AnubisBaboon::Update()
{
    if (m_bonusSpeedActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - m_abilityStartTime).count();

        if (elapsedTime >= 30) {
            // Restaurăm viteza inițială după 30 de secunde
            m_speed -= 5;
            m_bonusSpeedActive = false;
            std::cout << "Rage effect ended. Speed restored to " << m_speed << "\n";
        }
    }

    // Logica pentru cooldown, dacă este necesară
    if (m_remainingCooldown > 0) {
        m_remainingCooldown -= 1; // Presupunem că `update` este apelată la fiecare secundă
    }
}

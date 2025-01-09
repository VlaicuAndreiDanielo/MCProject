#include "BasicMonkey.h"

BasicMonkey::BasicMonkey() 
	: Character(500, 200, 12, 0)
{}

void BasicMonkey::ActivateSpecialAbility() 
{ //Special Ability = Quick Escape 
    if (m_remainingCooldown <= 0) {
        std::cout << "BasicMonkey activates Speed Boost!\n";

        // Temporar boost la viteză pentru 25 de secunde
        m_speed += 10;
        m_speedBoostActive = true;
        m_abilityStartTime = std::chrono::steady_clock::now();

        std::cout << "BasicMonkey gains +10 speed for 25 seconds!\n";
        std::cout << "New Speed: " << m_speed << "\n";

        // Setăm cooldown-ul abilității
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}

void BasicMonkey::MonkeyEvolution()
{
	if(EvolutionLevel < 5){
		std::cout << "BasicMonkey is evolving...\n";
		m_HP += 75;     // Crestem viata
		m_speed += 3;   // Crestem viteza
		EvolutionLevel++;
		std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
		std::cout << "Evolution Level: " << EvolutionLevel << "\n";
	}
	else {
		std::cout << "BasicMonkey has reached the maximum evolution level (5)!\n";
	}
}

void BasicMonkey::Update()
{
    if (m_speedBoostActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - m_abilityStartTime).count();

        if (elapsedTime >= 25) {
            // Restaurăm viteza inițială după 25 de secunde
            m_speed -= 10;
            m_speedBoostActive = false;
            std::cout << "Speed Boost effect ended. Speed restored to " << m_speed << "\n";
        }
    }

    // Logica pentru cooldown
    if (m_remainingCooldown > 0) {
        m_remainingCooldown -= 1; // Presupunem că `update` este apelată la fiecare secundă
    }
}

#include "Gorilla.h"

Gorilla::Gorilla()
	: Character(110, 135, 20, 0)
{}

void Gorilla::ActivateSpecialAbility() 
{   //Protection
    auto now = std::chrono::steady_clock::now();
    if (m_remainingCooldown <= 0) {
        std::cout << "Gorilla activates Shield!\n";

        // Activăm scutul de 30 de puncte
        m_HP += 30;
       
        m_abilityStartTime = now;

        std::cout << "Gorilla ate a banana, +20 hp points!\n";

        // Setăm cooldown-ul abilității la 20 de secunde
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::chrono::duration<float> elapsed = now - m_abilityStartTime;
        m_remainingCooldown = m_cooldownTime - elapsed.count();
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}



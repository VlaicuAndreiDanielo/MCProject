#include "BasicMonkey.h"

BasicMonkey::BasicMonkey() 
	: Character(75, 200, 15, 0)
{}

void BasicMonkey::ActivateSpecialAbility() 
{ //Special Ability = Quick Escape 
    auto now = std::chrono::steady_clock::now();
    if (m_remainingCooldown <= 0) {
        std::cout << "BasicMonkey activates Speed Boost!\n";

        // Temporar boost la viteză pentru 25 de secunde
        m_speed += 10;
        m_abilityStartTime = now;

        std::cout << "BasicMonkey ate a banana, gains +10 speed points!\n";
        std::cout << "New Speed: " << m_speed << "\n";

        // Setăm cooldown-ul abilității
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::chrono::duration<float> elapsed = now - m_abilityStartTime;
        m_remainingCooldown = m_cooldownTime - elapsed.count();
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds.\n";
    }
}


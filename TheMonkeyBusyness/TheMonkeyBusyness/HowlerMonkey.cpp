#include "HowlerMonkey.h"
#include <iostream>

HowlerMonkey::HowlerMonkey()
    : Character(600, 6, 30, 0), m_isInvulnerable(false), m_soundBarrierDuration(5), m_baseHP(600)
{}

void HowlerMonkey::ActivateSpecialAbility()
{   //Sound Barrier
    if (!m_isInvulnerable && m_remainingCooldown <= 0) {
        std::cout << "HowlerMonkey activates Sound Barrier! It's now invulnerable for 5 seconds.\n";
        m_isInvulnerable = true;
        m_remainingCooldown = m_cooldownTime;  // Setați cooldown-ul la 30 secunde
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}

void HowlerMonkey::Update()
{
    if (m_isInvulnerable) {
        // Contorizăm durata de invulnerabilitate
        m_soundBarrierDuration--;
        if (m_soundBarrierDuration <= 0) {
            // După ce timpul expiră, invulnerabilitatea se dezactivează
            std::cout << "Sound Barrier has ended. HowlerMonkey is now vulnerable.\n";
            m_isInvulnerable = false;
            m_soundBarrierDuration = 5;  // Resetăm durata pentru activarea viitoare
        }
    }

    // Reducem cooldown-ul dacă este activ
    if (m_remainingCooldown > 0) {
        m_remainingCooldown--;
    }
}

void HowlerMonkey::TakeDamage(int damage)
{
    if (!m_isInvulnerable) {
        m_HP -= damage;
        std::cout << "HowlerMonkey takes " << damage << " damage. Remaining HP: " << m_HP << "\n";
    }
    else {
        std::cout << "HowlerMonkey is invulnerable and takes no damage!\n";
    }
}

void HowlerMonkey::MonkeyEvolution()
{
    if (EvolutionLevel < 5) {
        std::cout << "HowlerMonkey is evolving...\n";
        m_HP += 60;  // Creștem viața
        m_speed += 2; // Creștem viteza
        EvolutionLevel++;
        std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
        std::cout << "Evolution Level: " << EvolutionLevel << "\n";
    }
    else {
        std::cout << "HowlerMonkey has reached the maximum evolution level (5)!\n";
    }
}

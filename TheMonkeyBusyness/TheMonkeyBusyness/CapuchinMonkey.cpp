#include "CapuchinMonkey.h"
#include <iostream>

CapuchinMonkey::CapuchinMonkey()
    : Character(400, 8, 15, 0), m_isAgilityActive(false), m_agilityDuration(15), m_agilityBoostAmount(5), m_baseSpeed(8)
{}

void CapuchinMonkey::ActivateSpecialAbility() {
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count() >= m_cooldownTime) {
        std::cout << "CapuchinMonkey activates its Special Ability!\n";
        m_lastAbilityUse = now;

        // Your special ability logic
        // Example: CapuchinMonkey gains a burst of speed
        m_speed += 5;
        std::cout << "Speed increased to: " << m_speed << "\n";

        // Reset ability after a short duration
        std::this_thread::sleep_for(std::chrono::seconds(3));
        m_speed -= 5;
        std::cout << "Speed normalized to: " << m_speed << "\n";
    }
    else {
        int timeLeft = m_cooldownTime - std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count();
        std::cout << "Ability is on cooldown. Time left: " << timeLeft << " seconds.\n";
    }
}

void CapuchinMonkey::Update()
{
    if (m_isAgilityActive) {
        // Contorizăm durata abilității
        m_agilityDuration--;
        if (m_agilityDuration <= 0) {
            // Odată ce durata expiră, revenim la viteza de bază
            std::cout << "Agility Boost has ended.\n";
            m_speed = m_baseSpeed;
            m_isAgilityActive = false;
            m_agilityDuration = 15;  // Resetăm durata pentru activarea viitoare
        }
    }

    // Scădem cooldown-ul, dacă e necesar
    if (m_remainingCooldown > 0) {
        m_remainingCooldown--;
    }
}

void CapuchinMonkey::MonkeyEvolution()
{
    if (EvolutionLevel < 5) {
        std::cout << "CapuchinMonkey is evolving...\n";
        m_HP += 100;  // Creștem viața
        m_speed += 3; // Creștem viteza
        m_baseSpeed = m_speed;
        EvolutionLevel++;
        std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
        std::cout << "Evolution Level: " << EvolutionLevel << "\n";
    }
    else {
        std::cout << "CapuchinMonkey has reached the maximum evolution level (5)!\n";
    }
}

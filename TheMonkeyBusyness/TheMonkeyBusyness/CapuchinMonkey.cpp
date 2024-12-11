#include "CapuchinMonkey.h"
#include <iostream>

CapuchinMonkey::CapuchinMonkey()
    : Character(400, 8, 15, 0), m_isAgilityActive(false), m_agilityDuration(15), m_agilityBoostAmount(5), m_baseSpeed(8)
{}

void CapuchinMonkey::ActivateSpecialAbility()
{   //Agility Boost
    if (!m_isAgilityActive && m_remainingCooldown <= 0) {
        std::cout << "CapuchinMonkey activates Agility Boost!\n";
        m_isAgilityActive = true;
        m_speed += m_agilityBoostAmount;  // Creștem viteza temporar
        m_remainingCooldown = m_cooldownTime;  // Setați timpul de cooldown
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
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

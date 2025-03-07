﻿#include "CapuchinMonkey.h"
#include <iostream>

CapuchinMonkey::CapuchinMonkey()
    : Character(80, 240, 5, 0)
{}

void CapuchinMonkey::ActivateSpecialAbility() {
    auto now = std::chrono::steady_clock::now();
    if (m_remainingCooldown <= 0) {
        std::cout << "CapuchinMonkey heals!\n";
        m_HP += 5;

        m_abilityStartTime = now;

        std::cout << "Capuchin ate a banana, +5 hp points!\n";

        // Setăm cooldown-ul abilității la 5 de secunde
        m_remainingCooldown = m_cooldownTime;
    }
    else {
        std::chrono::duration<float> elapsed = now - m_abilityStartTime;
        m_remainingCooldown = m_cooldownTime - elapsed.count();
        std::cout << "Ability is on cooldown. Time left: " << m_remainingCooldown << " seconds\n";
    }
}



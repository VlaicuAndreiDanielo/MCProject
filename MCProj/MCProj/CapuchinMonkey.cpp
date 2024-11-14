#include "CapuchinMonkey.h"
#include <iostream>

CapuchinMonkey::CapuchinMonkey()
    : Character(400, 8, 15, 0), isAgilityActive(false), agilityDuration(15), agilityBoostAmount(5), baseSpeed(8)
{}

void CapuchinMonkey::activateSpecialAbility()
{   //Agility Boost
    if (!isAgilityActive && remainingCooldown <= 0) {
        std::cout << "CapuchinMonkey activates Agility Boost!\n";
        isAgilityActive = true;
        speed += agilityBoostAmount;  // Creștem viteza temporar
        remainingCooldown = cooldownTime;  // Setați timpul de cooldown
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
    }
}

void CapuchinMonkey::update()
{
    if (isAgilityActive) {
        // Contorizăm durata abilității
        agilityDuration--;
        if (agilityDuration <= 0) {
            // Odată ce durata expiră, revenim la viteza de bază
            std::cout << "Agility Boost has ended.\n";
            speed = baseSpeed;
            isAgilityActive = false;
            agilityDuration = 15;  // Resetăm durata pentru activarea viitoare
        }
    }

    // Scădem cooldown-ul, dacă e necesar
    if (remainingCooldown > 0) {
        remainingCooldown--;
    }
}

void CapuchinMonkey::monkeyEvolution()
{
    if (evolutionLevel < 5) {
        std::cout << "CapuchinMonkey is evolving...\n";
        HP += 100;  // Creștem viața
        speed += 3; // Creștem viteza
        baseSpeed = speed;
        evolutionLevel++;
        std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
        std::cout << "Evolution Level: " << evolutionLevel << "\n";
    }
    else {
        std::cout << "CapuchinMonkey has reached the maximum evolution level (5)!\n";
    }
}

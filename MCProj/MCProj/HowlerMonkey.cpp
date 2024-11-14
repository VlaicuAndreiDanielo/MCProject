#include "HowlerMonkey.h"
#include <iostream>

HowlerMonkey::HowlerMonkey()
    : Character(600, 6, 30, 0), isInvulnerable(false), soundBarrierDuration(5), baseHP(600)
{}

void HowlerMonkey::activateSpecialAbility()
{   //Sound Barrier
    if (!isInvulnerable && remainingCooldown <= 0) {
        std::cout << "HowlerMonkey activates Sound Barrier! It's now invulnerable for 5 seconds.\n";
        isInvulnerable = true;
        remainingCooldown = cooldownTime;  // Setați cooldown-ul la 30 secunde
    }
    else {
        std::cout << "Ability is on cooldown. Time left: " << remainingCooldown << " seconds\n";
    }
}

void HowlerMonkey::update()
{
    if (isInvulnerable) {
        // Contorizăm durata de invulnerabilitate
        soundBarrierDuration--;
        if (soundBarrierDuration <= 0) {
            // După ce timpul expiră, invulnerabilitatea se dezactivează
            std::cout << "Sound Barrier has ended. HowlerMonkey is now vulnerable.\n";
            isInvulnerable = false;
            soundBarrierDuration = 5;  // Resetăm durata pentru activarea viitoare
        }
    }

    // Reducem cooldown-ul dacă este activ
    if (remainingCooldown > 0) {
        remainingCooldown--;
    }
}

void HowlerMonkey::takeDamage(int damage)
{
    if (!isInvulnerable) {
        HP -= damage;
        std::cout << "HowlerMonkey takes " << damage << " damage. Remaining HP: " << HP << "\n";
    }
    else {
        std::cout << "HowlerMonkey is invulnerable and takes no damage!\n";
    }
}

void HowlerMonkey::monkeyEvolution()
{
    if (evolutionLevel < 5) {
        std::cout << "HowlerMonkey is evolving...\n";
        HP += 60;  // Creștem viața
        speed += 2; // Creștem viteza
        evolutionLevel++;
        std::cout << "New stats - HP: " << HP << ", Speed: " << speed << "\n";
        std::cout << "Evolution Level: " << evolutionLevel << "\n";
    }
    else {
        std::cout << "HowlerMonkey has reached the maximum evolution level (5)!\n";
    }
}

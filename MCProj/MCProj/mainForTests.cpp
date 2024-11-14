#include <iostream>
#include "CapuchinMonkey.h"
#include "HowlerMonkey.h"
#include "Orangutan.h"
#include "Gorilla.h"
#include "AnubisBaboon.h"
#include "BasicMonkey.h"

int main() {
    // Cream instanțte pentru fiecare tip de maimuta
    CapuchinMonkey capuchin;
    HowlerMonkey howler;
    Orangutan orangutan;
    Gorilla gorilla;
    AnubisBaboon anubis;
    BasicMonkey basic;

    std::cout << "Maimuțe create cu succes!\n\n";

    // Activăm abilitățile speciale și facem update pentru fiecare maimuță:
    std::cout << "Activăm abilitățile speciale și facem update pentru fiecare maimuță:\n\n";

    // Capuchin Monkey
    std::cout << "Capuchin Monkey:\n";
    capuchin.activateSpecialAbility();
    capuchin.update();
    std::cout << "Health: " << capuchin.getHealth() << "\n";
    std::cout << "Speed: " << capuchin.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << capuchin.getCooldownTime() << "\n";
    capuchin.monkeyEvolution();

    // Howler Monkey
    std::cout << "\nHowler Monkey:\n";
    howler.activateSpecialAbility();
    howler.update();
    std::cout << "Health: " << howler.getHealth() << "\n";
    std::cout << "Speed: " << howler.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << howler.getCooldownTime() << "\n";
    howler.monkeyEvolution();

    // Orangutan
    std::cout << "\nOrangutan:\n";
    orangutan.activateSpecialAbility();
    std::cout << "Health: " << orangutan.getHealth() << "\n";
    std::cout << "Speed: " << orangutan.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << orangutan.getCooldownTime() << "\n";
    orangutan.monkeyEvolution();

    // Gorilla
    std::cout << "\nGorilla:\n";
    gorilla.activateSpecialAbility();
    std::cout << "Health: " << gorilla.getHealth() << "\n";
    std::cout << "Speed: " << gorilla.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << gorilla.getCooldownTime() << "\n";
    std::cout << "Shield: " << gorilla.getSheild() << "\n";
    gorilla.monkeyEvolution();

    // Anubis Baboon
    std::cout << "\nAnubis Baboon:\n";
    anubis.activateSpecialAbility();
    anubis.update();
    std::cout << "Health: " << anubis.getHealth() << "\n";
    std::cout << "Speed: " << anubis.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << anubis.getCooldownTime() << "\n";
    anubis.monkeyEvolution();

    // Basic Monkey
    std::cout << "\nBasic Monkey:\n";
    basic.activateSpecialAbility();
    basic.update();
    std::cout << "Health: " << basic.getHealth() << "\n";
    std::cout << "Speed: " << basic.getSpeed() << "\n";
    std::cout << "Cooldown Time: " << basic.getCooldownTime() << "\n";
    basic.monkeyEvolution();

    std::cout << "\nTest complet!\n";
    return 0;
}

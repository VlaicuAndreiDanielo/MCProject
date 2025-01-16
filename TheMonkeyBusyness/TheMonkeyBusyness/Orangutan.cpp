#include "Orangutan.h"

Orangutan::Orangutan() 
	: Character(85, 180, 20, 0)
{}

void Orangutan::ActivateSpecialAbility() {
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count() >= m_cooldownTime) {
		std::cout << "Orangutan activates Regeneration!\n";
		m_lastAbilityUse = now;

		// Regeneration logic
		int regenerationAmount = GetRandomHealthRegen(2, 5);
		m_HP += regenerationAmount;
		std::cout << "Orangutan regenerates " << regenerationAmount
			<< " HP. New HP: " << m_HP << "\n";
	}
	else {
		int timeLeft = m_cooldownTime - std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count();
		std::cout << "Ability is on cooldown. Time left: " << timeLeft << " seconds.\n";
	}
}

void Orangutan::MonkeyEvolution()
{
	if(EvolutionLevel<5){
		std::cout << "Orangutan is evolving...\n";
		m_HP += 50;     // Crestem viata
		m_speed += 2;   // Crestem viteza
		EvolutionLevel++;
		std::cout << "New stats - HP: " << m_HP << ", Speed: " << m_speed << "\n";
		std::cout << "Evolution Level: " << EvolutionLevel << "\n";
	}
	else {
		std::cout << "Orangutan has reached the maximum evolution level (5)!\n";
	}
}

int Orangutan::GetRandomHealthRegen(int min, int max) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

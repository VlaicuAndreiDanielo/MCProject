#include "Orangutan.h"

Orangutan::Orangutan() 
	: Character(90, 180, 15, 0)
{}

void Orangutan::ActivateSpecialAbility() {
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count() >= m_cooldownTime) {
		std::cout << "Orangutan activates Regeneration!\n";
		m_lastAbilityUse = now;

		// Regeneration logic
		int regenerationAmount = GetRandomHealthRegen(5, 20);
		m_HP += regenerationAmount;
		m_speed += (20 - regenerationAmount);
		std::cout << "Orangutan regenerates " << regenerationAmount
			<< " HP. New HP: " << m_HP << "\n";
	}
	else {
		int timeLeft = m_cooldownTime - std::chrono::duration_cast<std::chrono::seconds>(now - m_lastAbilityUse).count();
		std::cout << "Ability is on cooldown. Time left: " << timeLeft << " seconds.\n";
	}
}



int Orangutan::GetRandomHealthRegen(int min, int max) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

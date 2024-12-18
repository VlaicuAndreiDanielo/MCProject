#pragma once

#include <vector>
#include "Vector2.h"
#include "Bullet.h"
#include "ConstantValues.h"

//TODO serialize weapon. I should serialize the timers for the powerups and also add the serialization of bullets in the weapon class I think because bullets are f the weapon not of the player,check before doing.

class Weapon
{
public:
	Weapon(float damage=WeaponConfig::kBasicDamage,
		float fireRate=WeaponConfig::kBasicFireRate,
		float speed=WeaponConfig::kBasicSpeed);

	// Primary Actions
	void Shoot(const Vector2& position, const Vector2& direction);
	void Update(float deltaTime);
	void deactivateBullet(size_t index);

	// Power-up Management
	void ActivateDamagePowerup(float duration);
	void ActivateSpeedPowerup(float duration);

	// Getters
	float GetDamage() const;
	float GetFireRate() const;
	float GetSpeed() const;
	std::vector<Bullet>& GetActiveBullets();
	
	// Setters
	void SetDamage(float damage);
	void SetFireRate(float fireRate);
	void SetSpeed(float speed);

	crow::json::wvalue ToJson() const;

private:
	// Weapon properties
	float m_damage;
	float m_fireRate;
	float m_speed;

	// Bullet Management
	float m_timeSinceLastShot;   // Tracks cooldown between shots
	std::vector<Bullet> m_activeBullets;   // Active bullets in play
	std::vector<Bullet> m_inactiveBullets; // Pool of reusable bullets

	// Power-up timers
	float m_damageIncreaseTimer;
	float m_speedIncreaseTimer;

	// Private Helpers
	void initializeBullets();
	bool hasActivePowerup() const;                  // Checks if any power-up is active
	void activateBulletPowerup(Bullet& bullet) const;     // Applies power-up effects to a bullet if shot when power-ups are active
	void deactivateBulletsPowerup();          // Resets the bullet to default values when power-up is finished
	void updatePowerupsTimeLeft(float deltatTime);                          // Updates power-ups state
};
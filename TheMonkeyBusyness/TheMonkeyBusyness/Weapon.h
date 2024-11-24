#pragma once

#include "Vector2.h"
#include "Bullet.h"
#include <vector>
#include "weaponsConfig.h"

class Weapon
{
public:
	Weapon(float damage=WeaponConfig::kBasicDamage, float fireRate=WeaponConfig::kBasicFireRate, float speed=WeaponConfig::kBasicSpeed);

	void Shoot(const Vector2& position, const Vector2& direction);
	void Update();
	void DrawBullets(QPainter& painter) const; //TODO temporary for draw methods

	void ActivateDamagePowerup(float duration);
	void ActivateSpeedPowerup(float duration);

	float GetDamage() const;
	float GetFireRate() const;
	float GetSpeed() const;
	
	void SetDamage(float damage);
	void SetFireRate(float fireRate);
	void SetSpeed(float speed);

	std::vector<Bullet>& getBullets() { // temporary getter
		return m_activeBullets;
	}


private:
	float m_damage;
	float m_fireRate;
	float m_speed;

	float m_timeSinceLastShot;
	std::vector<Bullet> m_activeBullets;
	std::vector<Bullet> m_inactiveBullets;

	//attributes and methods for powerups
	float m_damageIncreaseTimer;
	float m_speedIncreaseTimer;

	bool hasActivePowerup() const;
	void activatePowerupOnBullet(Bullet& bullet);
	void updatePowerupTimers();
};
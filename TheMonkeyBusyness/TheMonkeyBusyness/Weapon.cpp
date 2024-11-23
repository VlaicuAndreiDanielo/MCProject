#include "Weapon.h"
#include "weaponsConfig.h"

Weapon::Weapon(float damage, float fireRate, float speed)
	: m_damage{ damage }, m_fireRate{ fireRate }, m_speed{ speed }, m_timeSinceLastShot{ 0.0f }, m_damageIncreaseTimer{ 0.0f }, m_speedIncreaseTimer{ 0.0f }
{
	/*
	the number of bullets that can exist at one time is limited by the fireRate
	It is more efficient to reserve and initialize all the possible bullets directly instead of creating them on the go
	Now using move semantics the bullets only need to be moved from active to inactive and vice versa

	Another idea was to have a single vector of bullets and a vector of indices for active/inactive bullets

	The complexity for the first method is
	- iterating over active bullets O(n)
	- moving a bullet from active to inactive std::move is O(1) pop_back is O(1) push_back is O(1) so final is O(1)

	The complexity for the second method is the same.The problem here is that indices will be mixed, if a player
	shoots 5 bullets,and the 3rd one hits something, then I will have active indices 1,2,4,5 and it will make bullet logic more complex
	In the first metod I always know where all the active/inactive bullets are.
	The first method is better because if offers a cleaner and more intuitive approach

	In the future make functions that go trough the inactive and active bullets and change their stats when monkey powerups are activated like damage increase.
	*/
	m_activeBullets.reserve(WeaponConfig::kMaxBasicBullets);
	m_inactiveBullets.resize(WeaponConfig::kMaxBasicBullets, Bullet(Vector2(), Vector2(), speed, damage));
}

void Weapon::Shoot(const Vector2& position, const Vector2& direction)
{
	if (m_timeSinceLastShot >= m_fireRate && !m_inactiveBullets.empty())
	{
		m_inactiveBullets.back().SetPosition(position);
		m_inactiveBullets.back().SetDirection(direction);
		m_activeBullets.push_back(std::move(m_inactiveBullets.back()));
		m_inactiveBullets.pop_back();

		m_timeSinceLastShot = 0.0f;

		/*if (hasActivePowerup)
			activatePowerupOnBullet(m_activeBullets.back());*/
	}
}

void Weapon::Update()
{
	m_timeSinceLastShot += WeaponConfig::frameTime; //TODO check if I should replace frameTime from constexpr to DEFIN

	for (auto& bullet : m_activeBullets) {
		bullet.Update();
		//TODO add logic for deactivating bullets once map and player classes are finished
	}
}

void Weapon::DrawBullets(QPainter& painter) const { //TODO temporary for draw methods
	for (const Bullet& bullet : m_activeBullets) {
		bullet.draw(painter);
	}
}

void Weapon::ActivateDamagePowerup(float duration)
{
	m_damageIncreaseTimer += duration;
}

void Weapon::ActivateSpeedPowerup(float duration)
{
	m_speedIncreaseTimer += duration;
}

bool Weapon::hasActivePowerup() const
{
	return m_damageIncreaseTimer > 0 || m_speedIncreaseTimer > 0;
}

void Weapon::activatePowerupOnBullet(Bullet& bullet)
{
	if (m_damageIncreaseTimer)
		bullet.SetDamage(bullet.GetDamage() + WeaponConfig::DamagePowerupIncreasePercent);
	if (m_speedIncreaseTimer)
		bullet.SetSpeed(bullet.GetSpeed() + WeaponConfig::speedPowerupIncreasePercent);
}

void Weapon::updatePowerupTimers()
{
	if (m_damageIncreaseTimer > 0)
		m_damageIncreaseTimer -= WeaponConfig::frameTime; //TODO check if I should replace frameTime from constexpr to DEFINE
	if (m_speedIncreaseTimer > 0)
		m_speedIncreaseTimer -= WeaponConfig::frameTime; //TODO same as above
}

float Weapon::GetDamage() const {
	return m_damage;
}

void Weapon::SetDamage(float damage) {
	m_damage = damage;
}

float Weapon::GetFireRate() const {
	return m_fireRate;
}

void Weapon::SetFireRate(float fireRate) {
	m_fireRate = fireRate;
}

float Weapon::GetSpeed() const {
	return m_speed;
}

void Weapon::SetSpeed(float speed) {
	m_speed = speed;
}
#include "Weapon.h"
#include "ConstantValues.h"

Weapon::Weapon(float damage, float fireRate, float speed)
	: m_damage{ damage }, m_fireRate{ fireRate }, m_speed{ speed }, m_timeSinceLastShot{ 0.0f }, m_damageIncreaseTimer{ 0.0f }, m_speedIncreaseTimer{ 0.0f }
{
	initializeBullets();
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

		if (hasActivePowerup())
			activateBulletPowerup(m_activeBullets.back());
	}
}

void Weapon::Update()
{
	m_timeSinceLastShot += frameTime;

	if (hasActivePowerup())
		updatePowerups();

	for (auto& bullet : m_activeBullets) {
		bullet.Update();
		//TODO add logic for deactivating bullets once map and player classes are finished
	}
}

void Weapon::initializeBullets()
{
	m_activeBullets.reserve(WeaponConfig::kMaxBasicBullets);
	m_inactiveBullets.reserve(WeaponConfig::kMaxBasicBullets);

	for (size_t i = 0; i < WeaponConfig::kMaxBasicBullets; ++i)
		m_inactiveBullets.push_back(Bullet(Vector2(), Vector2(), m_speed, m_damage));
}

//Va fi mutata mai incolo

//void Weapon::DrawBullets(QPainter& painter) const
//{
//	for (const Bullet& bullet : m_activeBullets)
//		bullet.draw(painter);
//}

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

void Weapon::activateBulletPowerup(Bullet& bullet) const
{
	if (m_damageIncreaseTimer)
		bullet.SetDamage(bullet.GetDamage() * (1 + WeaponConfig::kDamagePowerupIncreasePercent / 100.0f));
	if (m_speedIncreaseTimer)
		bullet.SetSpeed(bullet.GetSpeed() * (1 + WeaponConfig::kSpeedPowerupIncreasePercent / 100.0f));
}

void Weapon::deactivateBulletsPowerup()
{
	for(Bullet& bullet : m_activeBullets)
	{
		bullet.SetDamage(m_damage);
		bullet.SetSpeed(m_speed);
	}

	for (Bullet& bullet : m_inactiveBullets)
	{
		bullet.SetDamage(m_damage);
		bullet.SetSpeed(m_speed);
	}
}

void Weapon::updatePowerups()
{
	if (m_damageIncreaseTimer > 0)
	{
		m_damageIncreaseTimer -= frameTime;
		if (m_damageIncreaseTimer <= 0)
			deactivateBulletsPowerup();
	}

	if (m_speedIncreaseTimer > 0)
	{
		m_speedIncreaseTimer -= frameTime;
		if (m_speedIncreaseTimer <= 0)
			deactivateBulletsPowerup();
	}
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

/*
Logic for bullet management

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
*/
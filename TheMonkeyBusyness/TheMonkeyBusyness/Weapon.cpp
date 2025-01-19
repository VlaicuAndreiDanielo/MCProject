#include "Weapon.h"
#include "ConstantValues.h"

Weapon::Weapon(float damage, float fireRate, float speed)
	: m_damage{ damage }, m_fireRate{ fireRate }, m_speed{ speed }, m_timeSinceLastShot{ 0.0f }, m_damageIncreaseTimer{ 0.0f }, m_speedIncreaseTimer{ 0.0f }
{
	initializeBullets();
}

void Weapon::Shoot(const Vector2<float>& position, const Vector2<float>& direction) {

	if (m_timeSinceLastShot >= m_fireRate) {
		if (!m_inactiveBullets.empty()) {
			// Bullet activation logic

			m_inactiveBullets.back().SetPosition(position);
			m_inactiveBullets.back().SetDirection(direction);
			m_activeBullets.push_back(std::move(m_inactiveBullets.back()));
			m_inactiveBullets.pop_back();
			m_timeSinceLastShot = 0.0f;


			if (hasActivePowerup())
				activateBulletPowerup(m_activeBullets.back());
		}
	}
}



void Weapon::Update(float deltaTime)
{
    m_timeSinceLastShot += deltaTime;
    updatePowerupsTimeLeft(deltaTime);
}


void Weapon::deactivateBullet(size_t index)
{
	if (index < m_activeBullets.size())
	{
		m_inactiveBullets.push_back(std::move(m_activeBullets[index]));
		m_activeBullets.erase(m_activeBullets.begin() + index);
	}
}

void Weapon::initializeBullets()
{
	m_activeBullets.reserve(WeaponConfig::kMaxBasicBullets);
	m_inactiveBullets.reserve(WeaponConfig::kMaxBasicBullets);

	for (size_t i = 0; i < WeaponConfig::kMaxBasicBullets; ++i)
		m_inactiveBullets.push_back(Bullet(Vector2<float>(), Vector2<float>(), m_speed, m_damage));
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

void Weapon::updatePowerupsTimeLeft(float deltaTime)
{
	if (m_damageIncreaseTimer > 0)
	{
		m_damageIncreaseTimer -= deltaTime;
		if (m_damageIncreaseTimer <= 0)
			deactivateBulletsPowerup();
	}

	if (m_speedIncreaseTimer > 0)
	{
		m_speedIncreaseTimer -= deltaTime;
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

crow::json::wvalue Weapon::ToJson() const {
	crow::json::wvalue weaponJson;

	crow::json::wvalue bulletsJson = crow::json::wvalue::list();
	size_t bulletIndex = 0;
	for (const auto& bullet : m_activeBullets) {
		bulletsJson[bulletIndex++] = bullet.ToJson();
	}
	weaponJson["bullets"] = std::move(bulletsJson);

	return weaponJson;
}


std::vector<Bullet>& Weapon::GetActiveBullets()
{
	return m_activeBullets;
}
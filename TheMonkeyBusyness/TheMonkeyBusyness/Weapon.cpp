#include "Weapon.h"
#include "ConstantValues.h"

Weapon::Weapon(float damage, float fireRate, float speed)
	: m_damage{ damage }, m_fireRate{ fireRate }, m_speed{ speed }, m_timeSinceLastShot{ 0.0f }, m_damageIncreaseTimer{ 0.0f }, m_speedIncreaseTimer{ 0.0f }
{
	initializeBullets();
}

void Weapon::Shoot(const Vector2& position, const Vector2& direction) {
	std::ofstream logFile("server_log.txt", std::ios::app);

	logFile << "Weapon::Shoot called with Position=(" << position.x << ", " << position.y
		<< "), Direction=(" << direction.x << ", " << direction.y << ")" << std::endl;
	logFile << "Current cooldown: " << m_timeSinceLastShot << ", Fire rate: " << m_fireRate << std::endl;

	if (m_timeSinceLastShot >= m_fireRate) {
		if (!m_inactiveBullets.empty()) {
			// Bullet activation logic
			logFile << "Activating bullet: Position=(" << position.x << ", " << position.y
				<< "), Direction=(" << direction.x << ", " << direction.y << ")" << std::endl;

			m_inactiveBullets.back().SetPosition(position);
			m_inactiveBullets.back().SetDirection(direction);
			m_activeBullets.push_back(std::move(m_inactiveBullets.back()));
			m_inactiveBullets.pop_back();
			m_timeSinceLastShot = 0.0f;

			logFile << "Bullet added to m_activeBullets. Total active bullets: " << m_activeBullets.size() << std::endl;

			if (hasActivePowerup()) {
				logFile << "Activating powerup for bullet at Position=("
					<< m_activeBullets.back().GetPosition().x << ", "
					<< m_activeBullets.back().GetPosition().y << ")" << std::endl;
				activateBulletPowerup(m_activeBullets.back());
			}
		}
		else {
			logFile << "Weapon cannot shoot: No inactive bullets available." << std::endl;
		}
	}
	else {
		logFile << "Weapon cannot shoot: Fire rate cooldown active." << std::endl;
	}
}



void Weapon::Update(float deltaTime)
{
    m_timeSinceLastShot += deltaTime;

    // Open log file and append new log entries for debugging purposes
    std::ofstream logFile("server_log.txt", std::ios::app);
    logFile << "[" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << "] ";
    logFile << "Weapon::Update - deltaTime: " << deltaTime 
            << ", Accumulated timeSinceLastShot: " << m_timeSinceLastShot << std::endl;

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
		m_inactiveBullets.push_back(Bullet(Vector2(), Vector2(), m_speed, m_damage));
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

crow::json::wvalue Weapon::toJson() const {
	crow::json::wvalue weaponJson;

	crow::json::wvalue bulletsJson = crow::json::wvalue::list();
	size_t bulletIndex = 0;
	for (const auto& bullet : m_activeBullets) {
		bulletsJson[bulletIndex++] = bullet.toJson();
	}
	weaponJson["bullets"] = std::move(bulletsJson);
	//TODO also return time left in powerups so it can be displayed on screen

	return weaponJson;
}


std::vector<Bullet>& Weapon::GetActiveBullets()
{
	return m_activeBullets;
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
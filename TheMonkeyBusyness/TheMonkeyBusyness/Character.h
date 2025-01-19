//#pragma once
//#include <iostream>
//#include <chrono>
//#include <mutex>
//class Character
//{
//public:
//	Character() {};
//	Character(int hp, int speed,float cooldown , float remainingtime)
//		: m_HP(hp), m_speed(speed), m_cooldownTime(cooldown), m_remainingCooldown(remainingtime) {};
//	virtual void Attack() const {};
//
//	virtual void ActivateSpecialAbility() = 0;
//	virtual ~Character() = default;
//	void SetHealth(int value)  {
//		m_HP = value; }
//	int GetHealth() const {
//		return m_HP; }
//	int GetSpeed() const {
//				return m_speed; }
//	void SetSpeed(int value) {
//		m_speed = value;
//	}
//	float GetCooldownTime() const { return m_cooldownTime; }
//	
//protected:
//	int m_HP=0;
//	int m_speed=0;
//	float m_cooldownTime;
//	float m_remainingCooldown;
//};
//

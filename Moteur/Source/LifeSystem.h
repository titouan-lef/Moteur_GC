#pragma once
#include "Component.h"

class LifeSystem : public Component
{
public:
	LifeSystem();
	~LifeSystem();
	void IsHit(Entity* e);
	void IsDead();
	void SetLifePoit(float life) {
		m_lifePoint = life;
	}
	float& GetLifePoint() { return m_lifePoint; };
private:
	float m_lifePoint;
	bool m_isDead;
};


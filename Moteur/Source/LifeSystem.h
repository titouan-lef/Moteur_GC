#pragma once
#include "Component.h"

class LifeSystem : public Component
{
public:
	LifeSystem();
	~LifeSystem();

	virtual void Initialize() override;
	virtual void Update() override;

	void Damage(float damage);
	void Heal(float heal);

	void SetLifePoint(float life) {	m_lifePoint = life;	}
	float GetLifePoint() { return m_lifePoint; };

	void SetMaxLifePoint(float life) { m_maxLifePoint = life; }
	float GetMaxLifePoint() { return m_maxLifePoint; };
private:
	float m_lifePoint = 3;
	float m_maxLifePoint  = 3;

	bool m_isDead = false;
};


#include "Collider.h"
#include "Entity.h"
#include "LifeSystem.h"

LifeSystem::LifeSystem()
{
    m_isDead = false;
    m_lifePoint = 3;
	m_maxLifePoint = 3;
}

LifeSystem::~LifeSystem()
{
}

void LifeSystem::Initialize()
{
}

void LifeSystem::Update()
{
    if (m_isDead) {
		this->GetOwner()->Destroy();
	}
}

void LifeSystem::Damage(float damage)
{
    if (m_lifePoint - damage <= 0)
    {
		m_isDead = true;
	}
    else 
    {
		m_lifePoint -= damage;
	}
}

void LifeSystem::Heal(float heal)
{
    if (m_lifePoint + heal >= m_maxLifePoint)
    {
		m_lifePoint = m_maxLifePoint;
	}
	else
	{
		m_lifePoint += heal;
	}
}

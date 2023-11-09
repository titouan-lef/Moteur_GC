#include "Collider.h"
#include "Entity.h"

#include "LifeSystem.h"

LifeSystem::LifeSystem()
{
    m_isDead = false;
    m_lifePoint = 3;
}

LifeSystem::~LifeSystem()
{
}


void LifeSystem::IsHit(Entity* e)
{
    //for (auto caillou : *listRock)
    //{
    //    auto collider = caillou->GetComponent<Collider>();
    //    if (collider->CheckCollision(this->GetOwner()->GetComponent<Collider>())) {
    //        if (m_lifePoint - 1 <= 0) {
    //            m_isDead = true;
    //        }
    //        else {
    //            m_lifePoint--;
    //        };
    //    }
    //}
    if (e->GetComponent<Collider>()->CheckCollision(this->GetOwner()->GetComponent<Collider>())) {
        if (m_lifePoint - 1 <= 0) {
            m_isDead = true;
        }
        else {
            m_lifePoint--;
        };
    }
    IsDead();
}

void LifeSystem::IsDead()
{
    if (m_isDead) {
        std::cout << "Tu es mort !!";
        system("pause");
    }
}

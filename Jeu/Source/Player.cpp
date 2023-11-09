
#include <Camera.h>

#include <Collider.h>
#include <LifeSystem.h>
#include "Player.h"

Player::Player()
{
	m_controller = new Controller();
    m_time = new Timer();
    m_lifePoint = 3;
    m_isDead = false;

    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
    this->GetComponent<Transform>()->SetPosition(0, 0, 0.1);
    this->GetComponent<Transform>()->UpdateMatrix();
    this->AddComponent<Collider>();
    this->AddComponent<LifeSystem>();
}

Player::~Player()
{
	//delete m_controller;
}

void Player::Init()
{
}

void Player::Update()
{
    float elapsedTime = m_time->Peek();
    m_controller->Update();
   
   
    if (m_controller->IsMoving(Controller::Direction::Right)) {
         //Ajustez la rotation de la cam�ra vers la droit
            Camera::GetInstance()->GetComponent<Transform>()->Rotate(0,elapsedTime *2, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Left)) {
        //Ajustez la rotation de la cam�ra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(0, elapsedTime*-2, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Up)) {
        //Ajustez la rotation de la cam�ra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(elapsedTime * -2, 0, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Down)) {
        //Ajustez la rotation de la cam�ra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(elapsedTime * 2, 0, 0); // Ajoutez la logique de rotation ici
    }
    Camera::GetInstance()->RealUpdate();
    m_time->Mark();
}

void Player::IsHit(std::list<Entity*>* listRock)
{
    for (auto caillou : *listRock)
    {
        auto collider = caillou->GetComponent<Collider>();
        if (collider->CheckCollision(this->GetComponent<Collider>())) {
            if (m_lifePoint - 1 <= 0) {
                m_isDead = true;
            }
            else {
                m_lifePoint--;
            };
        }
    }
    IsDead();
}

void Player::IsDead()
{
    if (m_isDead) {
        system("pause");
    }
}

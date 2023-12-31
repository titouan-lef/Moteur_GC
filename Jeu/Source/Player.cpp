#include <Engine.h>
#include <Camera.h>
#include <Collider.h>
#include <LifeSystem.h>
#include "Shooter.h"
#include "Player.h"

Player::Player()
{
	m_controller = new Controller();
    m_time = new Timer();
    m_isDead = false;
}

Player::~Player()
{

}

void Player::Init()
{
    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
    this->GetComponent<Transform>()->SetPosition(0, 0, 0.1);
    this->AddComponent<Collider>();
    GetComponent<Collider>()->SetTag("Player");
    GetComponent<Collider>()->addListener(std::bind(&Player::OnCollision, 
        this, std::placeholders::_1));
    this->AddComponent<LifeSystem>();
    this->AddComponent<Shooter>();
    GetComponent<Shooter>()->SetController(m_controller);
    Engine::GetInstance()->SetPlayer(this);
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
    //IsDead();
}

void Player::OnCollision(Entity* e)
{
    if (e->GetComponent<Collider>()->GetTag() == "Asteroid")
    {
		//this->GetComponent<LifeSystem>()->Damage(1);
	}
}

void Player::IsDead()
{
    if (m_isDead) {
        std::wstring message = std::to_wstring((m_controller->m_coordMouse.y)) + L"\n";

        OutputDebugString(message.c_str());
        system("pause");
    }
}

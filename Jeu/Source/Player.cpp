#include <Camera.h>
#include <Collider.h>
#include <LifeSystem.h>
#include <Engine.h>
#include "Player.h"

Player::Player()
{
	m_controller = new Controller();
    m_time = new Timer();

    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
    this->GetComponent<Transform>()->SetPosition(0, 0, 0.1);
    this->GetComponent<Transform>()->UpdateMatrix();
    this->AddComponent<Collider>();
    GetComponent<Collider>()->SetTag("Player");
    //this->AddComponent<LifeSystem>();
    Engine::GetInstance()->SetPlayer(this);
}

Player::~Player()
{

}

void Player::Init()
{
    GetComponent<Collider>()->addListener(std::bind(&Player::OnCollision, 
        this, std::placeholders::_1));
}

void Player::Update()
{
    float elapsedTime = m_time->Peek();
    m_controller->Update();
   
   
    if (m_controller->IsMoving(Controller::Direction::Right)) {
         //Ajustez la rotation de la caméra vers la droit
            Camera::GetInstance()->GetComponent<Transform>()->Rotate(0,elapsedTime *2, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Left)) {
        //Ajustez la rotation de la caméra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(0, elapsedTime*-2, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Up)) {
        //Ajustez la rotation de la caméra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(elapsedTime * -2, 0, 0); // Ajoutez la logique de rotation ici
    }
    if (m_controller->IsMoving(Controller::Direction::Down)) {
        //Ajustez la rotation de la caméra vers la droit
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(elapsedTime * 2, 0, 0); // Ajoutez la logique de rotation ici
    }
    Camera::GetInstance()->RealUpdate();
    m_time->Mark();
}

void Player::OnCollision(Entity* e)
{
    if (e->GetComponent<Collider>()->GetTag() == "Asteroid")
    {
		//this->GetComponent<LifeSystem>()->Damage(1);
	}
}



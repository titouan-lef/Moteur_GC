#include <Camera.h>
#include <Collider.h>
#include <LifeSystem.h>
#include "Player.h"
#include "Bullet.h"
#include <DirectXMath.h>

Player::Player()
{
	m_controller = new Controller();
    m_time = new Timer();
    m_lifePoint = 3;
    m_isDead = false;

    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
    this->GetComponent<Transform>()->SetPosition(0, 0, 0);
    this->GetComponent<Transform>()->UpdateMatrix();
    this->AddComponent<Collider>();
    this->AddComponent<LifeSystem>();
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
    float elapsedTime = m_time->Peek();
    m_controller->Update();
   
    float globalRotationAngle = 0.0f;

    float test = 0;
    const float maxRotation = DirectX::XM_PIDIV2;
    const float minRotation = -maxRotation;

    if (m_controller->IsMoving(Controller::Direction::Right)) {
        // Ajustez l'angle global de rotation
        globalRotationAngle += elapsedTime * 2;
        // Appliquez la rotation à la caméra
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(0, 0.025, 0);
    }

    if (m_controller->IsMoving(Controller::Direction::Left)) {
        // Ajustez l'angle global de rotation
        globalRotationAngle -= elapsedTime * 2;

        // Appliquez la rotation à la caméra
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(0, -0.025, 0);
    }

    if (m_controller->IsMoving(Controller::Direction::Up)) {
        // Ajustez l'angle global de rotation
        globalRotationAngle -= elapsedTime * 2;
        test += 0.025;

        // Appliquez la rotation à la caméra
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(-0.025, 0, 0);
    }

    if (m_controller->IsMoving(Controller::Direction::Down)) {
        // Ajustez l'angle global de rotation
        globalRotationAngle += elapsedTime * 2;
        test -= 0.025;

        // Appliquez la rotation à la caméra
        Camera::GetInstance()->GetComponent<Transform>()->Rotate(0.025, 0, 0);
    }
    if (m_controller->IsMoving(Controller::Direction::LeftClick)) {
        Shoot();
    }

    std::wstring message = std::to_wstring(test)+ L"\n";

    OutputDebugString(message.c_str());

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

void Player::Shoot() {
    
    Bullet* bullet = new Bullet(m_controller->m_coordMouse.x, m_controller->m_coordMouse.y);
    this->AddChild(bullet);
}
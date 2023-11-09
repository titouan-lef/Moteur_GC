#include "Bullet.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <Collider.h>

Bullet::Bullet(int x, int y) {
    Transform* transform = this->AddComponent<Transform>();

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderColor::GetInstance());

    float newX;
    float newY;

    transform->SetScale(0.05f, 0.05f, 0.05f);
    
    if (x >= 600) {
        newX = (x - 600) / 1000.0;
        if (y >= 500) {
            newY = (y - 500) / 1000.0;
            transform->SetPosition(newX, -newY, 0.15);
        }
        else {
            newY = (-y + 500) / 1000.0;
            transform->SetPosition(newX, newY, 0.15);
        }        
    }
    
    else {
        newX = (-x + 600) / 1000.0;
        if (y >= 500) {
            newY = (y - 500) / 1000.0;
            transform->SetPosition(-newX, -newY, 0.15);
        }
        else {
            newY = (-y + 500) / 1000.0;
            transform->SetPosition(-newX, newY, 0.15);
        }
    }

    transform->UpdateMatrix();
    transform->SetDirection(0.0f, 0.0f, 0.002f);
    this->AddComponent<Collider>();
}

Bullet::~Bullet() {

}

void Bullet::Update() {
    Transform* transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);
}

bool Bullet::isDead() {
    return time.Peek() > lifeTime;
}
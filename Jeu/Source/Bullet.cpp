#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderTexture.h"
#include <Collider.h>
#include "Bullet.h"
#include <CollisionManager.h>

Bullet::Bullet(int x, int y) {
    time = new Timer();
    Transform* transform = this->AddComponent<Transform>();

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderTexture::GetInstance(), 1);

    float newX;
    float newY;

    transform->SetScale(0.05f, 0.05f, 0.05f);

    if (x >= 750) {
        newX = (x - 750) / 2000.0f;
        if (y >= 400) {
            newY = (y - 400) / 2000.0f;
            transform->SetPosition(newX, -newY, 0.15f);
        }
        else {
            newY = (-y + 400) / 2000.0f;
            transform->SetPosition(newX, newY, 0.15f);
        }        
    }
    
    else {
        newX = (-x + 750) / 2000.0f;
        if (y >= 400) {
            newY = (y - 400) / 2000.0f;
            transform->SetPosition(-newX, -newY, 0.15f);
        }
        else {
            newY = (-y + 400) / 2000.0f;
            transform->SetPosition(-newX, newY, 0.15f);
        }
    }

    transform->UpdateMatrix();
    transform->SetDirection(0.0f, 0.0f, 0.002f);
    this->AddComponent<Collider>();
    GetComponent<Collider>()->SetTag("Bullet");
}

Bullet::~Bullet() {

}

void Bullet::Init()
{
    GetComponent<Collider>()->addListener(std::bind(&Bullet::OnCollision,
        		this, std::placeholders::_1));
}

void Bullet::Update() {
    Transform* transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);

    if (isDead()) {
        this->Destroy();
    }
}

bool Bullet::isDead() {
    return time->Peek() > lifeTime;
}

void Bullet::OnCollision(Entity* other)
{
    if (other->GetComponent<Collider>()->GetTag() == "Asteroid") {
        std::cout << "Asteroid" << std::endl;
		this->Destroy();
	}
}

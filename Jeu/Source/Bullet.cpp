#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <Collider.h>
#include "Bullet.h"
#include <CollisionManager.h>

Bullet::Bullet(int x, int y) {
    time = new Timer();
    Transform* transform = this->AddComponent<Transform>();

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderColor::GetInstance());

    float newX;
    float newY;
    int demitailleWindowX = 750;
    int demitailleWindowY = 400;

    transform->SetScale(0.05f, 0.05f, 0.05f);

    if (x >= demitailleWindowX) {
        newX = (x - demitailleWindowX) / 2000.0f;
        if (y >= demitailleWindowY){
            newY = (y - demitailleWindowY) / 2000.0f;
            transform->SetPosition(newX, -newY, 0.15f);
        }
        else {
            newY = (-y + demitailleWindowY) / 2000.0f;
            transform->SetPosition(newX, newY, 0.15f);
        }        
    }
    
    else {
        newX = (-x + demitailleWindowX) / 2000.0f;
        if (y >= demitailleWindowY) {
            newY = (y - demitailleWindowY) / 2000.0f;
            transform->SetPosition(-newX, -newY, 0.15f);
        }
        else {
            newY = (-y + demitailleWindowY) / 2000.0f;
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

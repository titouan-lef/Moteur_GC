#include "asteroidCube.h"
#include "MeshRenderer.h"
#include "ShaderTexture.h"
#include <iostream>
#include <random>
#include <LifeSystem.h>
#include <Texture.h>

Asteroid::Asteroid() {
    std::random_device rd;  // Utilisez une source d'entropie matérielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par défaut

    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
    std::uniform_real_distribution<float> x(-1.0f, 1.0f);
    std::uniform_real_distribution<float> z(-1.0f, 1.0f);

  
    Transform* transform = this->AddComponent<Transform>();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderTexture::GetInstance(), 0);

    transform->SetScale(0.2f, 0.2f, 0.2f);

    transform->SetPosition(x(gen), y(gen), 1);
    transform->UpdateMatrix();

    auto pos = *transform->GetPosition();
    transform->SetDirection((-pos.x / 100), (-pos.y / 100), (-pos.z / 100));

    this->AddComponent<Collider>();
    GetComponent<Collider>()->SetTag("Asteroid");
    this->AddComponent<LifeSystem>();
}

Asteroid::~Asteroid() {

}

void Asteroid::Init()
{
	GetComponent<Collider>()->addListener(std::bind(&Asteroid::OnCollision,
        		this, std::placeholders::_1));

}

void Asteroid::Update() {
    auto transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);
    GetComponent<Transform>()->Rotate(
        1 * Engine::GetInstance()->Time->Peek(),
        1 * Engine::GetInstance()->Time->Peek(),
        1 * Engine::GetInstance()->Time->Peek()
    );
    if (isDeadByTime()) {
		this->Destroy();
	}
}

void Asteroid::OnCollision(Entity* other)
{
    if (other->GetComponent<Collider>()->GetTag() == "Bullet") {
		this->Destroy();
	}

    if (other->GetComponent<Collider>()->GetTag() == "Player") {
		this->Destroy();
	}
}

bool Asteroid::isDeadByTime() {
    return time.Peek() > 10;
}
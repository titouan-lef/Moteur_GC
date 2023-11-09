#include "asteroidCube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <iostream>
#include <random>
#include <LifeSystem.h>

Asteroid::Asteroid() {
    std::random_device rd;  // Utilisez une source d'entropie mat�rielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par d�faut

    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
    std::uniform_real_distribution<float> x(-1.0f, 1.0f);
    std::uniform_real_distribution<float> z(-1.0f, 1.0f);

    this->AddComponent<Transform>();
    auto transform = this->GetComponent<Transform>();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(world));

    transform->SetScale(0.2f, 0.2f, 0.2f);

    transform->SetPosition(x(gen), y(gen), 1);
    transform->UpdateMatrix();

    auto pos = transform->GetPosition();
    transform->SetDirection((-pos.x / 100), (-pos.y / 100), (-pos.z / 100));

    this->AddComponent<Collider>();
    GetComponent<Collider>()->SetTag("Asteroid");
    this->AddComponent<LifeSystem>();
}

Asteroid::~Asteroid() {

}

void Asteroid::Update() {
    auto transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);
    GetComponent<Transform>()->Rotate(
        1 * Engine::GetInstance()->Time->Peek(),
        1 * Engine::GetInstance()->Time->Peek(),
        1 * Engine::GetInstance()->Time->Peek()
    );
}

bool Asteroid::isDeadByTime() {
    return time.Peek() > 20;
}

void Asteroid::isTouch(Collider* collider) {
    if (this->GetComponent<Collider>()->CheckCollision(collider)) {
        isDead = true;
    }
}
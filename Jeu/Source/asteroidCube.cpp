#include "asteroidCube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <iostream>
#include <random>

Asteroid::Asteroid() {
    std::random_device rd;  // Utilisez une source d'entropie matérielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par défaut

    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
    std::uniform_real_distribution<float> x(-1.0f, 1.0f);
    std::uniform_real_distribution<float> z(-1.0f, 0.0f);

    this->AddComponent<Transform>();
    auto transform = this->GetComponent<Transform>();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(world));

    transform->SetScale(0.2f, 0.2f, 0.2f);

    transform->SetPosition(x(gen), y(gen), 1);
    transform->UpdateMatrix();
    transform->SetRotationSpeed(45, 35, 90);

    auto pos = transform->GetPosition();
    transform->SetDirection((- pos.x / 1000), (- pos.y / 1000), (- pos.z / 1000));

    this->AddComponent<Collider>();
}

Asteroid::~Asteroid() {

}

void Asteroid::Update() {
    auto transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);
}

bool Asteroid::isDeadByTime() {
    return time.Peek() > 20;
}

void Asteroid::isTouch(Collider* collider) {
    if (this->GetComponent<Collider>()->CheckCollision(collider)) {
        isDead = true;
    }
}
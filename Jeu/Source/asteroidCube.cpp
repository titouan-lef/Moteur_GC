#include "asteroidCube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <iostream>
#include <random>

Asteroid::Asteroid() {
    std::random_device rd;  // Utilisez une source d'entropie mat�rielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par d�faut

    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
    std::uniform_real_distribution<float> x(-1.0f, 1.0f);
    std::uniform_real_distribution<float> z(-1.0f, 1.0f);
    std::uniform_real_distribution<float> speed_y(-2.5f, 1.5f);
    std::uniform_real_distribution<float> speed_x(-1.3f, 2.0f);
    std::uniform_real_distribution<float> speed_z(-1.7f, 2.3f);
    this->AddComponent<Transform>();
    auto transform = this->GetComponent<Transform>();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(world));

    transform->SetScale(0.2f, 0.2f, 0.2f);

    transform->SetPosition(x(gen), y(gen), 1);
    transform->UpdateMatrix();
    transform->SetRotationSpeed(speed_x(gen), speed_y(gen), speed_z(gen));
    //Truc nul pour faire aller l'asteroid sur le joueur
    if (transform->GetPosition().x > 0.2) {
        if (transform->GetPosition().y > 0.2) {
            transform->SetDirection(-0.001f, -0.001f, -0.001f);
        }
        if (transform->GetPosition().y < -0.2) {
            transform->SetDirection(-0.001f, 0.001f, -0.001f);
        }
        else {
            transform->SetDirection(-0.001f, 0, -0.001f);
        }
    }
    if (transform->GetPosition().x < -0.2) {
        if (transform->GetPosition().y > 0.2) {
            transform->SetDirection(0.001f, -0.001f, -0.001f);
        }
        if (transform->GetPosition().y < -0.2) {
            transform->SetDirection(0.001f, 0.001f, -0.001f);
        }
        else {
            transform->SetDirection(0.001f, 0, -0.001f);
        }
    }
    else {
        transform->SetDirection(0, 0, -0.001f);
    }

    this->AddComponent<Collider>();
}

Asteroid::~Asteroid() {

}

void Asteroid::Update() {
    auto transform = this->GetComponent<Transform>();
    transform->MoveByVector(transform->GetDirection(), 1);
    GetComponent<Transform>()->Rotate(
        this->GetComponent<Transform>()->GetRotationSpeed().x * Engine::GetInstance()->Time->Peek(),
        this->GetComponent<Transform>()->GetRotationSpeed().y * Engine::GetInstance()->Time->Peek(),
        this->GetComponent<Transform>()->GetRotationSpeed().z * Engine::GetInstance()->Time->Peek()
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
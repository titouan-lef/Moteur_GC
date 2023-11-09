#include "asteroidCube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <iostream>
#include <random>
#include <LifeSystem.h>

AsteroidCube::AsteroidCube() {
    std::random_device rd;  // Utilisez une source d'entropie matérielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par défaut

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

    auto pos = transform->GetPosition();
    transform->SetDirection((-pos.x / 100), (-pos.y / 100), (-pos.z / 100));

    this->AddComponent<Collider>();
    this->AddComponent<LifeSystem>();

}

AsteroidCube::~AsteroidCube() {

}
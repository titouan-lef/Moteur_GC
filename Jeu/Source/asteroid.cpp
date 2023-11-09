#include "asteroid.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "ShaderColor.h"
#include <iostream>
#include <random>
#include <LifeSystem.h>

Asteroid::Asteroid() {
   
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
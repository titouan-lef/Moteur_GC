#include "MyGame.h"
#include <Collider.h>
#include <iostream>
#include <random>


MyGame::MyGame()
{
    time = new Timer();
    listRock = new std::list<Entity*>;
    /*
    c = new Cube();

    c->GetComponent<Transform>()->SetScale(1.0f, 1.0f,1.0f);
    c->GetComponent<Transform>()->SetPosition(-0.45f, 0, 5);
    c->GetComponent<Transform>()->UpdateMatrix();
    c->GetComponent<Transform>()->SetDirection(0.01f, 0, 0);
    c->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);

    c->AddComponent<Collider>();
    Transform colR1;
    colR1.SetPosition(-0.45f, 0, 1);
    colR1.SetScale(0.2f, 0.2f, 0.2f);
    colR1.SetDirection(0.01f, 0, 0);
    colR1.SetRotationSpeed(45, 35, 90);
    colR1.UpdateMatrix();
    c->GetComponent<Collider>()->SetCollider(colR1);

    c->Update();*/
}

MyGame::~MyGame()
{
}

void MyGame::Update()
{
    if (time->Peek() > 0.5) {
        listRock->push_back(CreateRock());
        time->Mark();
    }
    for (auto caillou : *listRock)
    {
        auto transform = caillou->GetComponent<Transform>();
        transform->MoveByVector(transform->GetDirection(), 1);
        transform->Rotate(transform->GetRotationSpeed().x, 0.01f, transform->GetRotationSpeed().z);
        caillou->RealUpdate();
    }
}


Cube* MyGame::CreateRock() {
    Cube* c = new Cube();

    std::random_device rd;  // Utilisez une source d'entropie matérielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par défaut

    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
    std::uniform_real_distribution<float> x(-1.0f, 1.0f);

    c->GetComponent<Transform>()->SetScale(0.2f, 0.2f, 0.2f);
    //c->GetComponent<Transform>()->SetPosition(-0.45f, 0, 1);
    c->GetComponent<Transform>()->SetPosition(x(gen), y(gen), 1);
    c->GetComponent<Transform>()->UpdateMatrix();

    //Truc nul pour faire aller l'asteroid sur le joueur
    if (c->GetComponent<Transform>()->GetPosition().x > 0.2) {
        if (c->GetComponent<Transform>()->GetPosition().y > 0.2) {
            c->GetComponent<Transform>()->SetDirection(-0.001f, -0.001f, -0.001f);
        }
        if (c->GetComponent<Transform>()->GetPosition().y < -0.2) {
            c->GetComponent<Transform>()->SetDirection(-0.001f, 0.001f, -0.001f);
        }
        else {
            c->GetComponent<Transform>()->SetDirection(-0.001f, 0, -0.001f);
        }
    }
    if (c->GetComponent<Transform>()->GetPosition().x < -0.2) {
        if (c->GetComponent<Transform>()->GetPosition().y > 0.2) {
            c->GetComponent<Transform>()->SetDirection(0.001f, -0.001f, -0.001f);
        }
        if (c->GetComponent<Transform>()->GetPosition().y < -0.2) {
            c->GetComponent<Transform>()->SetDirection(0.001f, 0.001f, -0.001f);
        }
        else {
            c->GetComponent<Transform>()->SetDirection(0.001f, 0, -0.001f);
        }
    }
    else {
        c->GetComponent<Transform>()->SetDirection(0, 0, -0.001f);
    }
    
    //c->GetComponent<Transform>()->SetDirection(0.0f, 0.0f, -0.001f);
    c->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);

    c->AddComponent<Collider>();
    Transform colR1;
    colR1.SetPosition(-0.45f, 0, 1);
    colR1.SetScale(0.2f, 0.2f, 0.2f);
    colR1.SetDirection(0.01f, 0, 0);
    colR1.SetRotationSpeed(45, 35, 90);
    colR1.UpdateMatrix();
    c->GetComponent<Collider>()->SetCollider(colR1);

    return c;
}

void MyGame::Render()
{
    if (!listRock->empty()) {
        for (auto caillou : *listRock)
        {
            caillou->Render();
        }
    }
}
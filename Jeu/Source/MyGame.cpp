#include <Collider.h>
#include <Camera.h>
#include <iostream>
#include <random>
#include "GameScene.h"
#include <SceneManager.h>
#include "MyGame.h"


MyGame::MyGame()
{
    m_time = new Timer();
    listRock = new std::list<Entity*>;
    m_controller = new Controller();
}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
    // All scenes here
    SceneManager::GetInstance()->AddScene<GameScene>();
}

//void MyGame::Update()
//{
//    m_controller->Update();
//    float elapsedTime = m_time->Peek();
//    if (elapsedTime > 0.5) {
//        listRock->push_back(CreateRock());
//        m_time->Mark();
//    }
//    for (auto caillou : *listRock)
//    {
//        auto transform = caillou->GetComponent<Transform>();
//        transform->MoveByVector(transform->GetDirection(), 1);
//        //transform->Rotate(transform->GetRotationSpeed().x, transform->GetRotationSpeed().y, transform->GetRotationSpeed().z);
//        transform->Rotate(45 * Engine::GetInstance()->Time->Peek() *0.1f, 0, 0);
//        caillou->RealUpdate();
//    }
//
//    if (m_controller->IsMoving(Controller::Direction::Right)) {
//        // Ajustez la rotation de la cam�ra vers la droite
//        Camera::GetInstance()->GetComponent<Transform>()->Rotate(45* Engine::GetInstance()->Time->Peek() *0.1f, 0, 0); // Ajoutez la logique de rotation ici
//    }
//    Camera::GetInstance()->RealUpdate();
//    Engine::GetInstance()->Time->Mark();
//}

//Cube* MyGame::CreateRock() {
//    Cube* c = new Cube();
//
//    //Al�atoire
//    std::random_device rd;  // Utilisez une source d'entropie mat�rielle si disponible
//    std::default_random_engine gen(rd()); // Utilisez le moteur par d�faut
//
//    std::uniform_real_distribution<float> y(-1.0f, 1.0f);
//    std::uniform_real_distribution<float> x(-1.0f, 1.0f);
//
//
//    c->GetComponent<Transform>()->SetScale(0.2f, 0.2f, 0.2f);
//    //c->GetComponent<Transform>()->SetPosition(-0.45f, 0, 1);
//    c->GetComponent<Transform>()->SetPosition(x(gen), y(gen), 1);
//    c->GetComponent<Transform>()->UpdateMatrix();
//    c->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);
//    //Truc nul pour faire aller l'asteroid sur le joueur
//    if (c->GetComponent<Transform>()->GetPosition().x > 0.2) {
//        if (c->GetComponent<Transform>()->GetPosition().y > 0.2) {
//            c->GetComponent<Transform>()->SetDirection(-0.001f, -0.001f, -0.001f);
//        }
//        if (c->GetComponent<Transform>()->GetPosition().y < -0.2) {
//            c->GetComponent<Transform>()->SetDirection(-0.001f, 0.001f, -0.001f);
//        }
//        else {
//            c->GetComponent<Transform>()->SetDirection(-0.001f, 0, -0.001f);
//        }
//    }
//    if (c->GetComponent<Transform>()->GetPosition().x < -0.2) {
//        if (c->GetComponent<Transform>()->GetPosition().y > 0.2) {
//            c->GetComponent<Transform>()->SetDirection(0.001f, -0.001f, -0.001f);
//        }
//        if (c->GetComponent<Transform>()->GetPosition().y < -0.2) {
//            c->GetComponent<Transform>()->SetDirection(0.001f, 0.001f, -0.001f);
//        }
//        else {
//            c->GetComponent<Transform>()->SetDirection(0.001f, 0, -0.001f);
//        }
//    }
//    else {
//        c->GetComponent<Transform>()->SetDirection(0, 0, -0.001f);
//    }
//
//    //c->GetComponent<Transform>()->SetDirection(0.0f, 0.0f, -0.001f);
//
//
//    c->AddComponent<Collider>();
//    return c;
//}

//void MyGame::Render()
//{
//    if (!listRock->empty()) {
//        for (auto caillou : *listRock)
//        {
//            caillou->Render();
//        }
//    }
//}



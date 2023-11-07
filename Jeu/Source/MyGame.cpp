#include "MyGame.h"
#include <Collider.h>

MyGame::MyGame()
{
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

    c->Update();
}

MyGame::~MyGame()
{
}

void MyGame::Update()
{
    c->GetComponent<Transform>()->MoveByVector({0, 0, 0.1}, 0.01f);
    c->GetComponent<Transform>()->Rotate(c->GetComponent<Transform>()->GetRotationSpeed().x, 0.01f, c->GetComponent<Transform>()->GetRotationSpeed().z);
    c->RealUpdate();
}

void MyGame::Render()
{
	c->Render();
}

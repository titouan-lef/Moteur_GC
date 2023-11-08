#include "TestSwitchScene.h"
#include <Cube.h>
#include "GameScene.h"
#include <SceneManager.h>

TestSwitchScene::TestSwitchScene()
{
}

void TestSwitchScene::Init()
{
    Cube* c = new Cube();
    c->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
    c->GetComponent<Transform>()->SetPosition(2, 2, 5);
    c->GetComponent<Transform>()->UpdateMatrix();
    c->GetComponent<Transform>()->SetDirection(0.01f, 0, 0);
    c->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);
    this->AddEntity(c);
}

void TestSwitchScene::Update()
{
	test++;
    if (test == 100) {
		SceneManager::GetInstance()->GoToScene<GameScene>();
	}
}

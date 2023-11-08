#include "GameScene.h"
#include <Cube.h>
#include <SceneManager.h>
#include "TestSwitchScene.h"

GameScene::GameScene()
{
}

void GameScene::Init()
{
	Cube* c = new Cube();
    c->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);
    c->GetComponent<Transform>()->SetPosition(0, 0, 5);
    c->GetComponent<Transform>()->UpdateMatrix();
    c->GetComponent<Transform>()->SetDirection(0.01f, 0, 0);
    c->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);
    this->AddEntity(c);
    std::cout << "GameScene initialized" << std::endl;
}

void GameScene::Update()
{
    test++;
    if (test == 100) {
        SceneManager::GetInstance()->GoToScene<TestSwitchScene>();
	}
}

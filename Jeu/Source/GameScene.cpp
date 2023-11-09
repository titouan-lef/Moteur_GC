#include <SceneManager.h>
#include <iostream>
#include <random>
#include "GameScene.h"

GameScene::GameScene()
{
}

void GameScene::Init()
{
    Player* player = new Player();
    this->AddEntity(player);
}

void GameScene::Update()
{
    /*if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        Bullet* b = new Bullet(0,0);
        this->AddEntity(b);
    }*/
    float elapsedTime = m_time.Peek();
    if (elapsedTime > respawnTime) {
        Asteroid* asteroid = new Asteroid();
        this->AddEntity(asteroid);
        m_time.Mark();
    }
}

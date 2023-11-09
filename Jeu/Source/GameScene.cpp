#include "GameScene.h"
#include <Cube.h>
#include <SceneManager.h>
#include <iostream>
#include <random>

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
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        GetCursorPos(&coordMouse);       //Get les co de la souris et les stock dans coordMouse

        Bullet* bullet = new Bullet(coordMouse.x, coordMouse.y);
        this->AddEntity(bullet);

    }

    float elapsedTime = m_time.Peek();
    if (elapsedTime > 2) {
        Asteroid* asteroid = new Asteroid();
        this->AddEntity(asteroid);
        m_time.Mark();
    }
}

bool GameScene::TestBulletDeath(Bullet& b) { return b.isDead(); }
bool GameScene::TestRockDeath(Asteroid& a) {
    if (a.isDeadByTime()) {
        return true;
    }
    if (a.isDead) {
        return true;
    }
    return false;
};
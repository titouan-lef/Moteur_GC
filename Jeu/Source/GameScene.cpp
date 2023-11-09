#include "GameScene.h"
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
    float elapsedTime = m_time.Peek();
    if (elapsedTime > respawnTime) {
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
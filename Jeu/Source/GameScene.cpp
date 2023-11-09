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
    /*std::random_device rd;  // Utilisez une source d'entropie mat�rielle si disponible
    std::default_random_engine gen(rd()); // Utilisez le moteur par d�faut

    std::uniform_real_distribution<float> x(-0.5f, 1.5f);
    std::uniform_real_distribution<float> y(-2.0f, 2.0f);
    std::uniform_real_distribution<float> z(-1.7f, 1.7f);
	Cube* c = new Cube();
    
    c->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);
    c->GetComponent<Transform>()->SetPosition(0, 0, 2);
    c->GetComponent<Transform>()->UpdateMatrix();
    c->GetComponent<Transform>()->SetDirection(0.01f, 0, 0);
    c->GetComponent<Transform>()->SetRotationSpeed(x(gen), y(gen),z(gen));
    this->AddEntity(c);*/
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
    if (elapsedTime > 0.5) {
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
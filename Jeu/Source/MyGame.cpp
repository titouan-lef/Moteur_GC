#include <Collider.h>
#include <LifeSystem.h>
#include <Camera.h>
#include <iostream>
#include <random>
#include "GameScene.h"
#include <SceneManager.h>
#include "MyGame.h"



MyGame::MyGame()
{
    m_time = new Timer();
    m_timeR = new Timer();
    listRock = new std::list<Entity*>;
    m_controller = new Controller();
}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
    SceneManager::GetInstance()->AddScene<GameScene>();
}
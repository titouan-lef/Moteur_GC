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

}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
    SceneManager::GetInstance()->AddScene<GameScene>();
}
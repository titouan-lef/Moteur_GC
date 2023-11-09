#pragma once

#include <Scene.h>
#include "Player.h"
#include "Bullet.h"
#include "asteroidCube.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene() {}

	virtual void Init() override;
	virtual void Update() override;
private:

	float respawnTime = 2.0;
	Timer m_time;
};
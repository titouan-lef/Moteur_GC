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
	static bool TestBulletDeath(Bullet& b);
	static bool TestRockDeath(Asteroid& b);
private:
	Timer m_time;
};
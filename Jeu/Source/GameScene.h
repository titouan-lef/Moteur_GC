#pragma once

#include <Scene.h>
#include "Player.h"
#include "Bullet.h"
#include "asteroidCube.h"
#include "asteroidBizarre.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene() {}

	virtual void Init() override;
	virtual void Update() override;
private:

	POINT coordMouse;
	Timer m_time;
};
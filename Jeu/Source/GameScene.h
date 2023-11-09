#pragma once

#include <Scene.h>
#include "Player.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene() {}

	virtual void Init() override;
	virtual void Update() override;

	int test = 0;
};
#pragma once
#include <Game.h>
#include <Cube.h>
#include "Timer.h"

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	void Update() override;
	void Render() override;
	Cube* CreateRock();
private:
	Timer* time = nullptr;
	std::list<Entity*> *listRock = nullptr;
};
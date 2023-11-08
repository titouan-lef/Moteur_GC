#pragma once
#include <Game.h>
#include <Cube.h>
#include <Timer.h>
#include "Controller.h"

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	void Update() override;
	void Render() override;
	Cube* CreateRock();
private:
	Timer* m_time = nullptr;
	std::list<Entity*>* listRock = nullptr;
	Controller* m_controller;
};


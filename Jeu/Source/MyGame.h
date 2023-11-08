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

	virtual void Initialize() override;

	void Update();
	void Render();
	Cube* CreateRock();
private:
	Timer* m_time = nullptr;
	std::list<Entity*>* listRock = nullptr;
	Controller* m_controller;
};

